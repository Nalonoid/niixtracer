#include "material_pbr.hpp"
#include "dielectric.hpp"
#include "metal.hpp"
#include "brdf.hpp"
#include "Utils/sampler.hpp"

MaterialPBR::MaterialPBR(const BRDF *brdf, std::string name) :
    _brdf(brdf), _name(name) {}

const std::string& MaterialPBR::name() const
{
    return _name;
}

const BRDF* MaterialPBR::brdf() const
{
    return _brdf;
}

float MaterialPBR::reflectance(const Vec3d &wi, const Vec3d &wo,
                                const Intersection &i,
                                const unsigned wavelength) const
{
    return _brdf->evaluate(wi, wo, i, wavelength);
}

namespace MaterialsPBR
{

const MaterialPBR *MATTE    { new Matte() };

// Index of Refraction / Roughness / Name
const MaterialPBR *GLASS    { new Dielectric(1.5f, 0.0f, "glass")       };
const MaterialPBR *DIAMOND  { new Dielectric(2.418f, 0.0f, "diamond")   };

// Index of Refraction / Name
const MaterialPBR *MIRROR   { new Metal(0.0f, "mirror") };
const MaterialPBR *METAL    { new Metal(0.3f)           };

const MaterialPBR* material(std::string name)
{
    if (name == "matte")
        return MaterialsPBR::MATTE;

    if (name == "mirror")
        return MaterialsPBR::MIRROR;

    if (name == "glass")
        return MaterialsPBR::GLASS;

    if (name == "metal")
        return MaterialsPBR::METAL;

    if (name == "diamond")
        return MaterialsPBR::DIAMOND;

    std::cerr << "error: invalid material name - nullptr returned" << std::endl;
    return nullptr;
}

}

// Matte class
Matte::Matte() : MaterialPBR(&BRDFs::LAMBERT, "matte") {}

Vec3d Matte::wi(const Vec3d&, Vec3d &normal) const
{
    // Uniform hemisphere sampling
    return Vec3d(rnd_dir_hemisphere(normal).normalized());
}

float Matte::roughness() const
{
    return 1.0f;
}
