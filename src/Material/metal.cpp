#include "metal.hpp"
#include "Utils/sampler.hpp"

Metal::Metal(float roughness, std::string name, const Spectrum<> *reflectance,
             const Fluorescence *fluorescence) :
    MaterialPBR(reflectance, name, fluorescence),
    _roughness (roughness) {}

float Metal::roughness() const
{
    return _roughness;
}

Vec3d Metal::wi(const Vec3d &wo, Vec3d &normal) const
{   
    float u { uniform_sampler_float.sample() };

    if (u > _roughness)
        return wo.reflect(normal);
    else
        return Vec3d(rnd_dir_hemisphere(normal));
}

float Metal::pdf(const Vec3d&, const Vec3d&,
                           const Intersection&) const
{
    // Only one possible direction for wi, hence the probability density function = 1
    return 1.0f;
}

