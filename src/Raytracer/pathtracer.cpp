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

        if (u < rr_stop_proba)
            _russian_roulette_coeff = 1.0 / (1.0 - rr_stop_proba);
    }

    return curr_depth > _scene->max_depth() && _russian_roulette_coeff != 1.0;
}

Color Pathtracer::compute_color(Ray &ray)
{   
    Color ret_color;

    switch (ray.intersection().material()->type())
    {
    case MATERIAL_TYPE::DIFFUSE:
        ret_color = compute_diffuse(ray);
        break;

    case MATERIAL_TYPE::SPECULAR:
        ret_color = compute_reflection(ray);
        break;

    case MATERIAL_TYPE::REFRACTIVE:
        ret_color = compute_refraction(ray);
        break;

    default:
        std::cerr << "error: wrong material type!" << std::endl;
        ret_color = Colors::BLACK;
        break;
    }

    return ret_color;
}

const Color Pathtracer::compute_diffuse(Ray &ray)
{
    Intersection &i { ray.intersection()    };
    const Shape *s  { i.shape()             };
    Color obj_col   { i.material()->color() };

    // Global illumination
    double cos_att { ray.direction().dot(i.normal()) };

    if (cos_att > 0.0)
        i.normal() = i.normal().negative();

    Vec3d recursive_dir { rnd_dir_hemisphere(i.normal()).normalized() };
    Ray recursive_ray(i.position() + EPSILON * recursive_dir, recursive_dir);
    recursive_ray.bounces() = ray.bounces() + 1;

    Color ret_color { launch(recursive_ray) * cos_att };

    // Without it we could not see the reflections of the source itself
    if (s->emits())
        ret_color += s->material().color();

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
                                (*shape_it)->material().color() * obj_col *
                                cosine_norm_light;
                }
            }
        }
    }

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
        Vec3d reflect_vect  { ray.direction().reflect(i.normal()) };
        reflect_vect = reflect_vect.normalized();

        Ray reflection_ray(i.position() + EPSILON * reflect_vect, reflect_vect);
        reflection_ray.bounces() = ray.bounces() + 1;

        ret_color = (Color(s->emission()) + launch(reflection_ray))
                * _russian_roulette_coeff;
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

    double n { i.material()->refraction() };
    double n1 { 1.0 };
    double n2 { 1.0 };

    /* For now we only consider reflection and refraction happening from air to
     * a second medium. Hence n1 = 1 as an approximationm.
     * That is why n = n1/n2 = 1.0/n2 here */
    if (normal.dot(ray.direction()) > 0.0)  // Inside the medium, going out
    {
        normal = normal.negative();
        n1 = n;
    }
    else
    {
        n2 = n;
        n = 1.0 / n;
    }

    double cos_R    { -ray.direction().dot(i.normal())      };
    double sin2_T   { n*n*(1 - cos_R*cos_R)                 };
    double u        { uniform_sampler.sample()              };
    double R        { schlick_approx(n1, n2, cos_R, sin2_T) };

    if (u > R)
    {
        double T { 1 - R };

        Vec3d refract_vect { n * ray.direction() +
                    (n * cos_R - sqrt(1.0 - sin2_T))*i.normal() };

        Ray refract_ray(i.position() + EPSILON * refract_vect, refract_vect);
        refract_ray.bounces() = ray.bounces() + 1;

        ret_color = (Color(s->emission()) + T * launch(refract_ray))
                * _russian_roulette_coeff;
    }
    else
        ret_color = compute_reflection(ray);

    return ret_color;
}
