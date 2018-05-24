#include <algorithm>

#include "pathtracer.hpp"
#include "ray.hpp"

#include "Image/color.hpp"
#include "Scene/scene.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/shape.hpp"
#include "Utils/sampler.hpp"
#include "Utils/utils.hpp"

#include "Material/glass.hpp"
#include "Material/matte.hpp"
#include "Material/mirror.hpp"

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
        double u                { uniform_sampler.sample() };
        double rr_stop_proba    { 0.1 };

        if (u < rr_stop_proba)
            _russian_roulette_coeff = 1.0 / (1.0 - rr_stop_proba);
    }

    return curr_depth > _scene->max_depth() && _russian_roulette_coeff != 1.0;
}

Color Pathtracer::compute_color(Ray &ray)
{   
    const MaterialPBR *m { ray.intersection().shape()->materialPBR() };

    const Glass   *glass  { dynamic_cast<const Glass*>(m)   };
    const Mirror  *mirror { dynamic_cast<const Mirror*>(m)  };
    const Matte   *matte  { dynamic_cast<const Matte*>(m)   };

    if (matte)
        return compute_diffuse(ray);

    if (mirror)
        return compute_reflection(ray);

    if (glass)
        return compute_refraction(ray);

    std::cerr << "error: wrong material type!" << std::endl;
    return Colors::BLACK;
}

const Color Pathtracer::compute_diffuse(Ray &ray)
{
    Intersection &i         { ray.intersection()    };
    const Shape *s          { i.shape()             };
    const MaterialPBR *m    { s->materialPBR()      };
    Color obj_col           { s->color()            };

    // Global illumination
    double cos_att { ray.direction().dot(i.normal()) };

    if (cos_att > 0.0)
        i.normal() = i.normal().negative();

    Vec3d recursive_dir { m->wi(ray.direction(), i.normal()) };
    Ray recursive_ray(i.position() + EPSILON * recursive_dir, recursive_dir);
    recursive_ray.bounces() = ray.bounces() + 1;

    Color ret_color { launch(recursive_ray) * cos_att };

    // Without it we could not see the reflections of the source itself
    if (s->emits())
        ret_color += s->color();

    // Next event estimation - Direct illumination
    const auto &shapes = _scene->shapes();
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
                        ret_color += 1/(2*PI) * (*shape_it)->emission() *
                                (*shape_it)->color() * obj_col *
                                cosine_norm_light;
                }
            }
        }
    }

    return ret_color * _russian_roulette_coeff;
}

const Color Pathtracer::compute_reflection(Ray &ray)
{
    Intersection &i         { ray.intersection()    };
    const Shape *s          { i.shape()             };
    const MaterialPBR *m    { s->materialPBR()      };

    Vec3d reflect_vect  { m->wi(ray.direction(), i.normal()) };
    float reflectance {
        m->reflectance(reflect_vect, ray.direction(), i, ray.wavelength()) };

    reflect_vect = reflect_vect.normalized();

    Ray reflection_ray(i.position() + EPSILON * reflect_vect, reflect_vect);
    reflection_ray.bounces() = ray.bounces() + 1;

    return (Color(s->emission()) + reflectance * launch(reflection_ray))
            * _russian_roulette_coeff;
}

const Color Pathtracer::compute_refraction(Ray &ray)
{
    Intersection &i         { ray.intersection()    };
    const Shape *s          { i.shape()             };
    const MaterialPBR *m    { s->materialPBR()      };

    Color ret_color;

    Vec3d refract_vect { m->wi(ray.direction(), i.normal()) };
    float transmitance {
        1 - m->reflectance(refract_vect, ray.direction(), i, ray.wavelength()) };

    Ray refract_ray(i.position() + EPSILON * refract_vect, refract_vect);
    refract_ray.bounces() = ray.bounces() + 1;

    ret_color = (Color(s->emission()) + transmitance * launch(refract_ray))
            * _russian_roulette_coeff;

    return ret_color;
}
