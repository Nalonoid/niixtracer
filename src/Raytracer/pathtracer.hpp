#ifndef __PATHTRACER_HPP__
#define __PATHTRACER_HPP__

#include "renderer.hpp"

class Pathtracer : public Renderer
{
public:
    Pathtracer(Scene *scene);

protected:
    bool    depth_recursion_over(Ray &ray) override;
    Color   compute_color(Ray &ray) override;

    Color   color_global_illumination(Ray &ray);
    Color   color_direct_illumination(Ray &ray);

    double _russian_roulette_coeff;
};

#endif
