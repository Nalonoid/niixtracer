#include "brdf.hpp"

BRDF::BRDF() {}

double LambertianModel::evaluate(const Vec3d&, const Vec3d&,
                                 const Intersection&,
                                 const unsigned wavelength) const
{
    return max(0.0, Spectra::CONSTANT_SPD.power_at(wavelength));
}

float LambertianModel::pdf(const Vec3d &wi, const Vec3d&, const Intersection &i)
{
    return std::abs(wi.dot(i.normal())) / PI;
}

double IdealRefraction::evaluate(const Vec3d&, const Vec3d&,
                                 const Intersection&,
                                 const unsigned wavelength) const
{
    return max(0.0, Spectra::CONSTANT_SPD.power_at(wavelength));
}

float IdealRefraction::pdf(const Vec3d&, const Vec3d&, const Intersection&)
{
    // Only one possible direction for wi, hence the probability density function = 1
    return 1.0f;
}

double IdealSpecular::evaluate(const Vec3d&, const Vec3d&,
                                 const Intersection&,
                                 const unsigned wavelength) const
{
    return max(0.0, Spectra::CONSTANT_SPD.power_at(wavelength));
}

float IdealSpecular::pdf(const Vec3d&, const Vec3d&, const Intersection&)
{
    // Only one possible direction for wi, hence the probability density function = 1
    return 1.0f;
}

namespace BRDFs
{

const BRDF *LAMBERT             { new LambertianModel() };
const BRDF *IDEAL_REFRACTION    { new IdealRefraction() };
const BRDF *IDEAL_SPECULAR      { new IdealSpecular()   };

}
