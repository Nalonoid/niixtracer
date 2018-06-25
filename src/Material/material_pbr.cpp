#include "material_pbr.hpp"
#include "dielectric.hpp"
#include "metal.hpp"
#include "Utils/sampler.hpp"

MaterialPBR::MaterialPBR(const Spectrum<> *reflectance, std::string name) :
    _reflectance(reflectance), _name(name) {}

const std::string& MaterialPBR::name() const
{
    return _name;
}

float MaterialPBR::reflectance(const Vec3d&, const Vec3d&,
                                const Intersection&,
                                const unsigned wavelength) const
{
    return max(0.0, _reflectance->power_at(wavelength));
}

void MaterialPBR::set_reflectance(const Spectrum<> *reflectance_spctr)
{
    _reflectance = reflectance_spctr;
}

namespace MaterialsPBR
{

MaterialPBR* material(std::string name)
{
    if (name == "matte")
        return new Matte();

    // Index of Refraction / Roughness / Name
    if (name == "diamond")
        return new Dielectric(2.418f, 0.0f, "diamond");

    if (name == "glass")
        return new Dielectric(1.500f, 0.0f, "glass");

    if (name == "translucent")
        return new Dielectric(1.020f, 0.0f, "translucent");

    // Roughness / Name
    if (name == "metal")
        return new Metal(0.3f, "metal");

    if (name == "mirror")
        return new Metal(0.0f, "mirror");

    if (name == "plastic")
        return new Metal(0.7f, "plastic");

    std::cerr << "error: invalid material name - nullptr returned" << std::endl;
    return nullptr;
}

}

// Matte class
Matte::Matte() : MaterialPBR(new ConstantSPD<>(), "matte") {}

Vec3d Matte::wi(const Vec3d&, Vec3d &normal) const
{
    // Uniform hemisphere sampling
    return Vec3d(rnd_dir_hemisphere(normal));
}

float Matte::pdf(const Vec3d&, const Vec3d&,
                           const Intersection&) const
{
    // Uniform hemisphere sampling PDF
    return 1/(2*PI);

    // Cosine weighted PDF
    //return std::abs(wi.dot(i.normal())) / PI;
}

float Matte::roughness() const
{
    return 1.0f;
}
