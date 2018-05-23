#ifndef __BRDF_HPP__
#define __BRDF_HPP__

#include <cmath>

#include "Raytracer/spectrum.hpp"
#include "Raytracer/intersection.hpp"
#include "Math/math.hpp"

class BRDF
{
public:
    BRDF();

    virtual double evaluate(const Vec3d &wi, const Vec3d &wo,
                                const Intersection &i,
                                const unsigned wavelength) const = 0;

    // BRDF's Probability Density Function given a sampled incident direction
    virtual float pdf(const Vec3d &wi, const Vec3d &wo,
                      const Intersection &i) = 0;
};

class LambertianModel : public BRDF
{
    double evaluate(const Vec3d &wi, const Vec3d &wo,
                        const Intersection &i,
                        const unsigned wavelength) const override;

    float pdf(const Vec3d &wi, const Vec3d &wo, const Intersection &i) override;
};

class IdealRefraction : public BRDF
{
    double evaluate(const Vec3d &wi, const Vec3d &wo,
                        const Intersection &i,
                        const unsigned wavelength) const override;

    float pdf(const Vec3d &wi, const Vec3d &wo, const Intersection &i) override;
};

class IdealSpecular : public BRDF
{
    double evaluate(const Vec3d &wi, const Vec3d &wo,
                        const Intersection &i,
                        const unsigned wavelength) const override;

    float pdf(const Vec3d &wi, const Vec3d &wo, const Intersection &i) override;
};

namespace BRDFs
{

extern const BRDF *LAMBERT;
extern const BRDF *IDEAL_REFRACTION;
extern const BRDF *IDEAL_SPECULAR;

}

#endif
