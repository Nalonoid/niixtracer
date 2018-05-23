#ifndef __MATTE_HPP__
#define __MATTE_HPP__

#include "material_brdf.hpp"

class Matte : public MaterialBRDF
{
public:
    Matte();

    Vec3d wi(const Vec3d &wo, Vec3d &normal) const override;
};

#endif
