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
    BRDF(Spectrum<SPECTRAL_SAMPLES> *spectrum);

    virtual double evaluate(const Vec3d &wi, const Vec3d &wo,
                                const Intersection &i,
                                const unsigned wavelength) const = 0;

    // BRDF's Probability Density Function given a sampled incident direction
    virtual float pdf(const Vec3d &wi, const Vec3d &wo,
                      const Intersection &i) const = 0;

    Spectrum<SPECTRAL_SAMPLES> *_spectrum;
};

class LambertianModel : public BRDF
{
public:
    LambertianModel(ConstantSPD<> *spectrum = new ConstantSPD<>(0.6));

    double evaluate(const Vec3d &wi, const Vec3d &wo,
                        const Intersection &i,
                        const unsigned wavelength) const override;

    float pdf(const Vec3d &wi, const Vec3d &wo,
              const Intersection &i) const override;
};

class IdealRefraction : public BRDF
{
public:
    IdealRefraction(ConstantSPD<> *spectrum = new ConstantSPD<>(0.6));

    double evaluate(const Vec3d &wi, const Vec3d &wo,
                        const Intersection &i,
                        const unsigned wavelength) const override;

    float pdf(const Vec3d &wi, const Vec3d &wo,
              const Intersection &i) const override;
};

class IdealSpecular : public BRDF
{
public:
    IdealSpecular(ConstantSPD<> *spectrum = new ConstantSPD<>(0.6));

    double evaluate(const Vec3d &wi, const Vec3d &wo,
                        const Intersection &i,
                        const unsigned wavelength) const override;

    float pdf(const Vec3d &wi, const Vec3d &wo,
              const Intersection &i) const override;
};

namespace BRDFs
{

extern const LambertianModel LAMBERT;
extern const IdealSpecular   IDEAL_SPECULAR;
extern const IdealRefraction IDEAL_REFRACTION;

}


#endif
