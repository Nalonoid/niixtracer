#ifndef __MIRROR_HPP__
#define __MIRROR_HPP__

#include "material_brdf.hpp"

class Mirror : public MaterialBRDF
{
public:
    Mirror(float glossiness = 1.0f);

    Vec3d wi(const Vec3d &wo, Vec3d &normal) const override;

private:
    float _roughness;
};

#endif
