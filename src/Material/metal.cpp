#include "metal.hpp"
#include "brdf.hpp"
#include "Utils/sampler.hpp"

Metal::Metal(float roughness, std::string name) :
    MaterialPBR(&BRDFs::IDEAL_SPECULAR, name),
    _roughness (roughness) {}

float Metal::roughness() const
{
    return _roughness;
}

Vec3d Metal::wi(const Vec3d &wo, Vec3d &normal) const
{   
    return wo.reflect(normal);
}
