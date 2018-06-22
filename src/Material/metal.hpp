#ifndef __METAL_HPP__
#define __METAL_HPP__

#include "material_pbr.hpp"

class Metal : public MaterialPBR
{
public:
    Metal(float roughness = 0.3f, std::string name = "metal",
          const Spectrum<> *brdf = new ConstantSPD<>());

    float roughness() const override;
    Vec3d wi(const Vec3d &wo, Vec3d &normal) const override;
    float pdf(const Vec3d &wi, const Vec3d &wo,
              const Intersection &i) const override;

private:
    float _roughness;
};

#endif
