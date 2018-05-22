#ifndef __BRDF_HPP__
#define __BRDF_HPP__

#include "Raytracer/spectrum.hpp"
#include "Raytracer/intersection.hpp"
#include "Math/math.hpp"

class BRDF
{
public:
    BRDF();

    virtual Spectrum<> evaluate(
            const Vec3d &wi, const Vec3d &wo, const Intersection &i) const = 0;

    // BRDF's Probability Density Function given a sampled incident direction
    virtual float pdf(const Vec3d &wi, const Vec3d &wo,
                      const Intersection &i) = 0;
};

#endif
