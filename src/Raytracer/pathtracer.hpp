#ifndef __PATHTRACER_HPP__
#define __PATHTRACER_HPP__

#include "renderer.hpp"

class Pathtracer : public Renderer
{
public:
    Pathtracer(Scene *scene);

    bool depth_recursion_over(Ray &ray) override;
    Color compute_color(Ray &ray) override;

    const Color compute_refl_refractive(Ray &ray);

private:
    double _russian_roulette_coeff;
};

#endif
