#include "brdf.hpp"

BRDF::BRDF() : _spectrum(nullptr) {}

BRDF::BRDF(Spectrum<SPECTRAL_SAMPLES> *spectrum) : _spectrum(spectrum) {}

LambertianModel::LambertianModel(ConstantSPD<> *spectrum) :
    BRDF(spectrum) {}

IdealRefraction::IdealRefraction(ConstantSPD<> *spectrum) :
    BRDF(spectrum) {}

IdealSpecular::IdealSpecular(ConstantSPD<> *spectrum) :
    BRDF(spectrum) {}

double LambertianModel::evaluate(const Vec3d&, const Vec3d&,
                                 const Intersection&,
                                 const unsigned wavelength) const
{
    std::cout << "evaluate lambert " << _spectrum->power_at(wavelength) << std::endl;
    return max(0.0, _spectrum->power_at(wavelength));
}

float LambertianModel::pdf(const Vec3d &wi, const Vec3d&, const Intersection &i)
{
    return std::abs(wi.dot(i.normal())) / PI;
}

double IdealRefraction::evaluate(const Vec3d&, const Vec3d&,
                                 const Intersection&,
                                 const unsigned wavelength) const
{
    return max(0.0, _spectrum->power_at(wavelength));
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
    return max(0.0, _spectrum->power_at(wavelength));
}

float IdealSpecular::pdf(const Vec3d&, const Vec3d&, const Intersection&)
{
    // Only one possible direction for wi, hence the probability density function = 1
    return 1.0f;
}

namespace BRDFs
{

const LambertianModel   LAMBERT             { LambertianModel() };
const IdealRefraction   IDEAL_REFRACTION    { IdealRefraction() };
const IdealSpecular     IDEAL_SPECULAR      { IdealSpecular()   };

}
