#ifndef __PATHTRACER_HPP__
#define __PATHTRACER_HPP__

#include "renderer.hpp"

class Pathtracer : public Renderer
{
public:
    Pathtracer(Scene *scene);

    bool depth_recursion_over(Ray &ray) override;
    Color compute_color(Ray &ray) override;

private:
    double _russian_roulette_coeff;
};

#endif
