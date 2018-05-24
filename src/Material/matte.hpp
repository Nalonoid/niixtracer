#ifndef __MATTE_HPP__
#define __MATTE_HPP__

#include "material_pbr.hpp"

class Matte : public MaterialPBR
{
public:
    Matte();

    Vec3d wi(const Vec3d &wo, Vec3d &normal) const override;
};

#endif
