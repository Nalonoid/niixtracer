#ifndef __MIRROR_HPP__
#define __MIRROR_HPP__

#include "material_pbr.hpp"

class Mirror : public MaterialPBR
{
public:
    Mirror(float glossiness = 1.0f);

    // Getters
    float roughness() const override;

    Vec3d wi(const Vec3d &wo, Vec3d &normal) const override;

private:
    float _roughness;
};

#endif
