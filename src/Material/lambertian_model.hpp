#ifndef __LAMBERTIAN_MODEL_HPP__
#define __LAMBERTIAN_MODEL_HPP__

#include "brdf.hpp"

class LambertianModel : BRDF
{
    Spectrum<> evaluate(const Vec3d &wi,
                        const Vec3d &wo, const Intersection &i) const override;

    float pdf(const Vec3d &wi, const Vec3d &wo, const Intersection &i) override;
};

#endif
