#ifndef __MATERIAL_BRDF_HPP__
#define __MATERIAL_BRDF_HPP__

#include "Math/math.hpp"
#include "Raytracer/intersection.hpp"

class BRDF;

class MaterialBRDF
{
public:
    MaterialBRDF();
    MaterialBRDF(const BRDF *brdf);

    const BRDF* brdf() const;
    virtual Vec3d wi(const Vec3d &wo, Vec3d &normal) const = 0;
    float reflectance(const Vec3d &wi, const Vec3d &wo, const Intersection &i,
                      const unsigned wavelength) const;

private:
    const BRDF *_brdf;
};

namespace MaterialsBRDF
{

extern const MaterialBRDF *MATTE;
extern const MaterialBRDF *GLASS;
extern const MaterialBRDF *MIRROR;

}

#endif
