#ifndef __SPECTRAL_PATHTRACER_HPP__
#define __SPECTRAL_PATHTRACER_HPP__

#include "pathtracer.hpp"

class SpectralPathtracer : public Pathtracer
{
public:
    SpectralPathtracer(Scene *scene);
    float   radiance_along_path(Ray &ray);

protected:
    Color   compute_color(Ray &ray) override;

    float   radiance(Ray &ray);
    float   radiance_global_illumination(Ray &ray);
    float   radiance_direct_illumination(Ray &ray);
};

#endif
