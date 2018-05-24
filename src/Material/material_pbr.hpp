#ifndef __MATERIAL_BRDF_HPP__
#define __MATERIAL_BRDF_HPP__

#include "Math/math.hpp"

class Intersection;
class BRDF;

class MaterialPBR
{
public:
    MaterialPBR(const BRDF &brdf);

    // Getters
    const std::string& name() const;
    const BRDF& brdf() const;
    virtual float roughness() const;

    // Methods
    virtual Vec3d wi(const Vec3d &wo, Vec3d &normal) const = 0;
    float reflectance(const Vec3d &wi, const Vec3d &wo, const Intersection &i,
                      const unsigned wavelength) const;

private:
    std::string _name;
    const BRDF &_brdf;
};

namespace MaterialsPBR
{

extern const MaterialPBR *MATTE;
extern const MaterialPBR *GLASS;
extern const MaterialPBR *MIRROR;

const MaterialPBR* material(std::string name);

}

#endif
