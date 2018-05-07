#include <algorithm>

#include "pathtracer.hpp"
#include "ray.hpp"

#include "Image/color.hpp"
#include "Scene/scene.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/shape.hpp"
#include "Utils/sampler.hpp"

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
    const Intersection &i   { ray.intersection()    };

    Color ret_color;

    switch (i.material()->type())
    {
    case MATERIAL_TYPE::DIFFUSE:
        return compute_diffuse(ray);

    case MATERIAL_TYPE::SPECULAR:
        return compute_reflection(ray);

    case MATERIAL_TYPE::REFRACTIVE:
    {
        return compute_diffuse(ray);
        break;
    }
    default:
        compute_diffuse(ray);
        break;
    }

    return ret_color;
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

const Color Pathtracer::compute_diffuse(Ray &ray)
{
    const Intersection &i   { ray.intersection()    };
    const Shape *s          { i.shape()             };

    Color ret_color, obj_col { i.material()->color() };

    Vec3d x_axis, y_axis;
    Space::orthonormal_basis(i.normal(), x_axis, y_axis);

    Vec3d hsphere_smpl  { hemisphere_sample() };
    Vec3d recursive_dir {
        Vec3d(x_axis.x, y_axis.x, i.normal().x).dot(hsphere_smpl),
        Vec3d(x_axis.y, y_axis.y, i.normal().y).dot(hsphere_smpl),
        Vec3d(x_axis.z, y_axis.z, i.normal().z).dot(hsphere_smpl) };

    Ray recursive_ray(i.position() + EPSILON*recursive_dir, recursive_dir);
    recursive_ray.bounces() = ray.bounces() + 1;

    double cos_att { recursive_dir.dot(i.normal()) };

    ret_color = Color(s->emission()) * _russian_roulette_coeff +
            obj_col * launch(recursive_ray) * cos_att *
            _russian_roulette_coeff;

    return ret_color;
}
