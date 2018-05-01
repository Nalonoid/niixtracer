#include "pathtracer.hpp"
#include "ray.hpp"

#include "Image/color.hpp"
#include "Scene/scene.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/shape.hpp"
#include "Utils/sampler.hpp"

Pathtracer::Pathtracer(Scene *scene) : Renderer(scene),
    _russian_roulette_coeff(1.0f) {}

bool Pathtracer::depth_recursion_over(Ray &ray)
{
    unsigned curr_depth { ray.bounces() };

    _russian_roulette_coeff = 1.0;

    if (curr_depth > _scene->max_depth())
    {
        Uniform sampler;
        double u { sampler.sample() };
        double rr_stop_proba { std::min(0.0625 * curr_depth, 1.0) };

        if (u > rr_stop_proba)
            _russian_roulette_coeff /= (1.0 - rr_stop_proba);
    }

    return curr_depth > _scene->max_depth() && _russian_roulette_coeff == 1.0;
}

Color Pathtracer::compute_color(Ray &ray)
{
    const Shape *s = ray.intersection().shape();

    return Color(s->emission()) * 2;
}
