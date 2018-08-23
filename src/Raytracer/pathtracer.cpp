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
        double rr_stop_proba    { uniform_sampler_double.sample() };

        if (u < rr_stop_proba)
            _russian_roulette_coeff = 1.0 / (1.0 - rr_stop_proba);
    }

    return curr_depth > _scene->max_depth() && _russian_roulette_coeff != 1.0;
}

Color Pathtracer::compute_color(Ray &ray)
{
    Intersection &i         { ray.intersection()    };
    const Shape *s          { i.shape()             };

    // We stop the path when we hit a light source
    if (s->emits())
        return _russian_roulette_coeff * s->emission() * s->color();

    return color_global_illumination(ray) * s->color();
}

Color Pathtracer::color_global_illumination(Ray &ray)
{
    Intersection &i         { ray.intersection()    };
    const Shape *s          { i.shape()             };
    const MaterialPBR *m    { s->materialPBR()      };
    Vec3d rdir              { ray.direction()       };

    float cos_att { float(rdir.dot(i.normal())) };

    if (cos_att > 0.0)
        i.normal() = i.normal().negative();
    else
        cos_att = -cos_att;

    Vec3d recurs_dir { m->wi(rdir, i.normal()) };
    Ray recursive_ray(i.position() + EPSILON * recurs_dir, recurs_dir,
                      ray.bounces() + 1);

    float reflectance   { m->reflectance(recurs_dir, rdir, i)               };
    Color global        { reflectance * launch(recursive_ray) * cos_att     };
    global /= m->pdf(recurs_dir, rdir, i);

    return global;
}
