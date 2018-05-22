#include "lambertian_model.hpp"
#include <cmath>

Spectrum<> LambertianModel::evaluate(const Vec3d&, const Vec3d&,
                                     const Intersection&) const
{
    return ConstantSPD<>();
}

float LambertianModel::pdf(const Vec3d &wi, const Vec3d&,
                           const Intersection &i)
{
    return std::abs(wi.dot(i.normal())) / PI;
}
