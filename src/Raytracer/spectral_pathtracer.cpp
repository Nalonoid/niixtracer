#include <algorithm>

#include "spectral_pathtracer.hpp"
#include "ray.hpp"

#include "Image/color.hpp"
#include "Scene/scene.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/shape.hpp"
#include "Utils/sampler.hpp"
#include "Utils/utils.hpp"

#include "Material/dielectric.hpp"
#include "Material/metal.hpp"

SpectralPathtracer::SpectralPathtracer(Scene *scene) : Pathtracer(scene) {}

Color SpectralPathtracer::compute_color(Ray &ray)
{
    Spectrum<SPECTRAL_SAMPLES> spectral_radiance;

    // Integration over the wavelengths
    for (unsigned l {0}; l < SPECTRAL_SAMPLES; ++l)
    {
        unsigned lambda { l * SPECTRAL_RES + MIN_WAVELENGTH };

        ray.wavelength()        = lambda;
        spectral_radiance[l]    = radiance(ray);
    }

    return correct_gamma(spectral_radiance.to_RGB(_scene->base_illuminant_SPD()));
}

float SpectralPathtracer::radiance(Ray &ray)
{
    Intersection &i { ray.intersection() };
    const Shape *s  { i.shape()          };
    unsigned lambda { ray.wavelength()   };

    // We stop the path when we hit a light source
    if (s->emits())
        return s->emission(lambda);

    const MaterialPBR *m    { s->materialPBR()                  };
    float radiance          { radiance_global_illumination(ray) };

    // If the material is not matte we do not explicitly sample the light
    if (dynamic_cast<const Matte*>(m) == nullptr)
        return radiance * _russian_roulette_coeff;

    radiance += radiance_direct_illumination(ray);

    return _russian_roulette_coeff * radiance;
}

float SpectralPathtracer::radiance_along_path(Ray &ray)
{
    bool collides { false };

    if (depth_recursion_over(ray))
        return 0.0f;

    for (auto itr = shapes().begin(); itr < shapes().end(); itr++)
        collides = (*itr)->intersect(ray) || collides;

    if (!collides)
        return 0.0f;

    return radiance(ray);
}

float SpectralPathtracer::radiance_global_illumination(Ray &ray)
{
    Intersection &i         { ray.intersection()    };
    const Shape *s          { i.shape()             };
    const MaterialPBR *m    { s->materialPBR()      };
    unsigned lambda         { ray.wavelength()      };
    Vec3d rdir              { ray.direction()       };

    float cos_att { float(rdir.dot(i.normal())) };

    if (cos_att > 0.0)
        i.normal() = i.normal().negative();
    else
        cos_att = -cos_att;

    Vec3d recursive_dir;
    float u { uniform_sampler_float.sample() };

    if (u > m->roughness())
        recursive_dir = m->wi(rdir, i.normal()).normalized();
    else
        return radiance_direct_illumination(ray); // Diffuse reflection

    Ray recursive_ray(i.position() + EPSILON * recursive_dir, recursive_dir,
                      lambda);

    recursive_ray.bounces() = ray.bounces() + 1;

    float reflectance   { m->reflectance(recursive_dir, rdir, i, lambda)    };
    float glob_radiance { reflectance * radiance_along_path(recursive_ray)  };
    glob_radiance /= m->pdf(recursive_dir, rdir, i);

    return glob_radiance;
}

// Next event estimation - Here we perform "Explicit Light Sampling"
float SpectralPathtracer::radiance_direct_illumination(Ray &ray)
{
    Intersection &i     { ray.intersection()    };
    const Shape *s      { i.shape()             };
    unsigned lambda     { ray.wavelength()      };
    Vec3d rdir          { ray.direction()       };
    float dir_radiance  { 0.0f                  };

    const auto &shapes  { _scene->shapes()      };
    /* We sample all the light sources. An alternative would be to sample one
     * randomly chosen light source and multiply the result by the number of
     * light sources in the scene. Monte Carlo integration ensures that it will
     * still compute the correct result on average. */
    for (auto shape_it = shapes.begin(); shape_it < shapes.end(); shape_it++)
    {
        // If the shape is an emitter, we'll check for direct illumination
        if ((*shape_it)->emits())
        {
            Vec3d light_sample { (*shape_it)->position()+hemisphere_sample() };

            Vec3d cone_sample       { light_sample - i.position()   };
            double source_distance  { cone_sample.magnitude()       };

            cone_sample = cone_sample.normalized();

            double cosine_norm_light { i.normal().dot(cone_sample) };

            /* We don't need to compute direct illumination if normal of the
             * intersection doesn't point towards light */
            if (cosine_norm_light > 0.0)
            {
                Ray shadow_ray(i.position() + EPSILON*cone_sample, cone_sample);
                bool in_shadow = false;

                auto source_intersection =
                    std::find_if(shapes.begin(), shapes.end(), [&](Shape *shp)
                    {
                        return shp->intersect(shadow_ray)
                               && shadow_ray.dist_max() < source_distance
                               && shp != s;
                    });

                if (source_intersection != shapes.end())
                {
                    in_shadow = !(*source_intersection)->emits();

                    if (!in_shadow)
                    {
                        float reflectance   {
                            s->materialPBR()->reflectance(cone_sample, rdir, i, lambda) };

                        dir_radiance += (*source_intersection)->emission(lambda)
                                * reflectance * cosine_norm_light * 5.0;
                    }
                }
            }
        }
    }

    return dir_radiance;
}
