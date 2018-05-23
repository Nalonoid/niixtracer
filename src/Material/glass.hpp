#ifndef __GLASS_HPP__
#define __GLASS_HPP__

#include "material_brdf.hpp"

class Glass : public MaterialBRDF
{
public:
    Glass(float ior = 1.5f);

    Vec3d wi(const Vec3d &wo, Vec3d &normal) const override;

private:
    float _ior;   // Index of Refraction of the medium
};

#endif
