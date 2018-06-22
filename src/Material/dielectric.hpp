#ifndef __DIELECTRIC_HPP__
#define __DIELECTRIC_HPP__

#include "material_pbr.hpp"

class Dielectric : public MaterialPBR
{
public:
    Dielectric(float ior = 1.5f, float roughness = 0.0f,
               std::string name = "dielectric",
               const Spectrum<> *reflectance = new ConstantSPD<>());

    float roughness() const override;
    Vec3d wi(const Vec3d &wo, Vec3d &normal) const override;
    float pdf(const Vec3d &wi, const Vec3d &wo,
              const Intersection &i) const override;

private:
    float _ior;         // Index of Refraction of the medium
    float _roughness;         // Roughness of the material
};

#endif
