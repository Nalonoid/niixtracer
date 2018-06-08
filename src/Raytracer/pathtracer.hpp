#ifndef __PATHTRACER_HPP__
#define __PATHTRACER_HPP__

#include "renderer.hpp"

class Pathtracer : public Renderer
{
public:
    Pathtracer(Scene *scene);

    bool depth_recursion_over(Ray &ray) override;
    float launch(Ray &ray);
    float radiance(Ray &ray);
    Color compute_color(Ray &ray) override;

private:
    float diffuse_radiance(Ray &ray);

private:
    double _russian_roulette_coeff;
};

#endif
