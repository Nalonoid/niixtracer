#include <algorithm>

#include "pathtracer.hpp"
#include "ray.hpp"

#include "Image/color.hpp"
#include "Scene/scene.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/shape.hpp"
#include "Utils/sampler.hpp"
#include "Utils/utils.hpp"

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

        if (u > rr_stop_proba)
            _russian_roulette_coeff = 1.0 / (1.0 - rr_stop_proba);
    }

    return curr_depth > _scene->max_depth() && _russian_roulette_coeff == 1.0;
}

Color Pathtracer::compute_color(Ray &ray)
{   
    const Intersection &i { ray.intersection()    };

    switch (i.material()->type())
    {
    case MATERIAL_TYPE::DIFFUSE:
    {
        return compute_diffuse(ray);
    }
    case MATERIAL_TYPE::SPECULAR:
        return compute_reflection(ray);

    case MATERIAL_TYPE::REFRACTIVE:
        return compute_refraction(ray);

    default:
        std::cerr << "error: wrong material type!" << std::endl;
        return Colors::BLACK;
        break;
    }
}

const Color Pathtracer::compute_diffuse(Ray &ray)
{
    const Intersection &i       { ray.intersection()    };
    const Shape *s              { i.shape()             };
    Color ret_color, obj_col    { i.material()->color() };

//    //TO-DO Explicit light sampling
//    const auto &shapes = _scene->shapes();
//    for (auto shape_it = shapes.begin(); shape_it < shapes.end(); shape_it++)
//    {
//        // If the shape is an emitter, we'll check for direct illumination
//        if ((*shape_it)->emits())
//        {
//            bool directly_illuminated = false;
//            Vec3d towards_light {
//                ((*shape_it)->position() - i.position()).normalized() };

//            Vec3d cone_sample { (towards_light +
//                        rnd_dir_hemisphere(-1.0*towards_light)).normalized() };

//            double cos_att { i.normal().dot(cone_sample) };

//            /* We don't need to compute direct illumination if normal of the
//             * intersection doesn't point towards light */
//            if (cos_att > 0.0)
//            {
//                Ray shadow_ray(i.position() + EPSILON*cone_sample, cone_sample);

//                auto source_intersection =
//                    std::find_if(shapes.begin(), shapes.end(), [&](Shape *shp)
//                    {
//                        return shp->intersect(shadow_ray);
//                    });

//                directly_illuminated = source_intersection == shape_it;

//                if (directly_illuminated)
//                    ret_color +=
//                            (*shape_it)->emission() * obj_col * cos_att;
//            }
//        }
//    }

    // Global illumination
    Vec3d recursive_dir { rnd_dir_hemisphere(i.normal()) };
    Ray recursive_ray(i.position() + EPSILON*recursive_dir, recursive_dir);
    recursive_ray.bounces() = ray.bounces() + 1;

    double cos_att { recursive_dir.dot(i.normal()) };

    ret_color = Color(s->emission()) +
            (obj_col * launch(recursive_ray) * cos_att);

    return ret_color * _russian_roulette_coeff;
}

const Color Pathtracer::compute_reflection(Ray &ray)
{
    const Intersection &i   { ray.intersection()    };
    const Shape *s          { i.shape()             };

    Color ret_color;

    double u { uniform_sampler.sample()     };
    double R { i.material()->reflection()   };

    if (u <= R) // The reflection is in the perfect specular direction
    {
        Vec3d reflect_vect { ray.direction().reflect(i.normal()) };
        Ray reflection_ray(i.position() + EPSILON * reflect_vect, reflect_vect);
        reflection_ray.bounces() = ray.bounces() + 1;

        ret_color = Color(s->emission()) * _russian_roulette_coeff +
                launch(reflection_ray) * _russian_roulette_coeff;
    }
    else // Here we take a random direction on the hemisphere (diffuse)
        ret_color = compute_diffuse(ray);

    return ret_color;
}

const Color Pathtracer::compute_refraction(Ray &ray)
{
    const Intersection &i   { ray.intersection()    };
    const Shape *s          { i.shape()             };
    Vec3d normal            { i.normal()            };

    Color ret_color;

    double n    { i.material()->refraction() };

    /* For now we only consider reflection and refraction happening from air to
     * a second medium. Hence n1 = 1 as an approximationm.
     * That is why n = n1/n2 = 1.0/n2 here */
    if (normal.dot(ray.direction()) > 0.0)
        normal = normal.negative();
    else
        n = 1.0 / i.material()->refraction();

    double cos_R    { -ray.direction().dot(i.normal())      };
    double sin2_T   { n*n*(1 - cos_R*cos_R)                 };
    double u        { uniform_sampler.sample()              };
    double R        { schlick_approx(1.0, n, cos_R, sin2_T) };

    if (u > R)
    {
        double T { 1 - R };

        Vec3d refract_vect { n * ray.direction() +
                    (n * cos_R - sqrt(1.0 - sin2_T))*i.normal() };
        Ray refract_ray(i.position() + EPSILON*refract_vect, refract_vect);
        refract_ray.bounces() = ray.bounces() + 1;

        ret_color = Color(s->emission()) * _russian_roulette_coeff +
                T * _russian_roulette_coeff * launch(refract_ray);
    }
    else
        ret_color = compute_reflection(ray);

    return ret_color;
}
