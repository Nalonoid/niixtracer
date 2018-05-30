#include <algorithm>

#include "pathtracer.hpp"
#include "ray.hpp"

#include "Image/color.hpp"
#include "Scene/scene.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/shape.hpp"
#include "Utils/sampler.hpp"
#include "Utils/utils.hpp"

#include "Material/dielectric.hpp"
#include "Material/metal.hpp"
#include "Material/brdf.hpp"

Pathtracer::Pathtracer(Scene *scene) : Renderer(scene),
    _russian_roulette_coeff(1.0f) {}

/* https://computergraphics.stackexchange.com/questions/2316/is-russian-roulette-really-the-answer
 * see the above link for a nice explanation on how russian roulettes works and
 * why it is mathematically unbiased */
bool Pathtracer::depth_recursion_over(Ray &ray)
{
    unsigned curr_depth { ray.bounces() };

    _russian_roulette_coeff = 1.0;

    if (curr_depth > _scene->max_depth())
    {
        double u                { uniform_sampler_double.sample() };
        double rr_stop_proba    { 0.1 };

        if (u < rr_stop_proba)
            _russian_roulette_coeff = 1.0 / (1.0 - rr_stop_proba);
    }

    return curr_depth > _scene->max_depth() && _russian_roulette_coeff != 1.0;
}

Color Pathtracer::compute_color(Ray &ray)
{
    Intersection &i         { ray.intersection()    };
    const Shape *s          { i.shape()             };
    const MaterialPBR *m    { s->materialPBR()      };

    float u { uniform_sampler_float.sample() };
    Vec3d reflect_vect;

    // Reflection is in the ideal specular/refractive direction (depends on m)
    if (u > m->roughness())
        reflect_vect = m->wi(ray.direction(), i.normal());
    else
        return compute_diffuse(ray);    // Diffuse reflection

    float reflectance {
        m->reflectance(reflect_vect, ray.direction(), i, ray.wavelength()) };

    reflect_vect = reflect_vect.normalized();

    Ray reflection_ray(i.position() + EPSILON * reflect_vect, reflect_vect);
    reflection_ray.bounces() = ray.bounces() + 1;

    return 1/m->brdf()->pdf(reflect_vect, ray.direction(), i) *
            (Color(s->emission()) + reflectance * launch(reflection_ray))
            * _russian_roulette_coeff;
}


const Color Pathtracer::compute_diffuse(Ray &ray)
{
    Intersection &i         { ray.intersection()    };
    const Shape *s          { i.shape()             };
    const MaterialPBR *m    { s->materialPBR()      };
    Color obj_col           { s->color()            };

    // We stop the path when we hit a light source
    if (s->emits())
        return obj_col;

    // Global illumination
    double cos_att { ray.direction().dot(i.normal()) };

    if (cos_att > 0.0)
        i.normal() = i.normal().negative();
    else
        cos_att = -cos_att;

    Vec3d recursive_dir { m->wi(ray.direction(), i.normal()) };
    Ray recursive_ray(i.position() + EPSILON * recursive_dir, recursive_dir);
    recursive_ray.bounces() = ray.bounces() + 1;

    Color ret_color { launch(recursive_ray) * cos_att };

    // Next event estimation - Direct illumination
    const auto &shapes = _scene->shapes();

    /* Here we sample all the light sources. An alternative would be to sample
     * one randomly chosen light source and multiply the result by the number of
     * light sources in the scene. Monte Carlo integration ensures that it will
     * still compute the correct result on average. */
    for (auto shape_it = shapes.begin(); shape_it < shapes.end(); shape_it++)
    {
        // If the shape is an emitter, we'll check for direct illumination
        if ((*shape_it)->emits())
        {
            Vec3d light_sample { (*shape_it)->position()+hemisphere_sample() };
            Vec3d cone_sample { light_sample - i.position() };
            double source_distance  { cone_sample.magnitude() };
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
                        float reflectance {
                            m->reflectance(recursive_dir, ray.direction(), i,
                                           ray.wavelength()) };

                        ret_color *= reflectance;
                        ret_color += (*shape_it)->emission() *
                                (*shape_it)->color() * obj_col *
                                cosine_norm_light;
                        ret_color /= 2*PI;
                    }
                }
            }
        }
    }

    return ret_color;
}
