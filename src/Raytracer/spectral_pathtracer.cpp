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

    return correct_gamma(
                spectral_radiance.to_RGB(_scene->base_illuminant_SPD()));
}

float SpectralPathtracer::radiance(Ray &ray)
{
    Intersection &i { ray.intersection() };
    const Shape *s  { i.shape()          };
    unsigned lambda { ray.wavelength()   };

    // We stop the path when we hit a light source
    if (s->emits())
        return s->emission(lambda);

    return _russian_roulette_coeff * radiance_global_illumination(ray);
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

    Vec3d recursive_dir { m->wi(rdir, i.normal()) };
    Ray recursive_ray(i.position() + EPSILON * recursive_dir, recursive_dir,
                      lambda);

    recursive_ray.bounces() = ray.bounces() + 1;

    float reflectance   { m->reflectance(recursive_dir, rdir, i, lambda)    };
    float glob_radiance { reflectance * radiance_along_path(recursive_ray)  };
    glob_radiance /= m->pdf(recursive_dir, rdir, i);

    return s->emission(lambda) + glob_radiance;
}
