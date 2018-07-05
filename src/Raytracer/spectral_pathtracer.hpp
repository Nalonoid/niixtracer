#ifndef __SPECTRAL_PATHTRACER_HPP__
#define __SPECTRAL_PATHTRACER_HPP__

#include "pathtracer.hpp"

class SpectralPathtracer : public Pathtracer
{
public:
    SpectralPathtracer(Scene *scene);

protected:
    Color   compute_color(Ray &ray) override;

    float   launch(Ray &ray);
    float   radiance(Ray &ray);
    float   radiance_global_illumination(Ray &ray);
    //float   radiance_direct_illumination(Ray &ray);
};

#endif
