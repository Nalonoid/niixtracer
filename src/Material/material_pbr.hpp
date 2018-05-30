#ifndef __MATERIAL_BRDF_HPP__
#define __MATERIAL_BRDF_HPP__

#include "Math/math.hpp"

class Intersection;
class BRDF;

class MaterialPBR
{
public:
    MaterialPBR(const BRDF *brdf, std::string name);

    // Getters
    const std::string& name() const;
    const BRDF* brdf() const;
    virtual float roughness() const = 0;

    // Methods
    virtual Vec3d wi(const Vec3d &wo, Vec3d &normal) const = 0;
    float reflectance(const Vec3d &wi, const Vec3d &wo, const Intersection &i,
                      const unsigned wavelength) const;

private:
    const BRDF *_brdf;
    std::string _name;
};

class Matte : public MaterialPBR
{
public:
    Matte();
    Vec3d wi(const Vec3d &wo, Vec3d &normal) const override;
    float roughness() const override;
};

namespace MaterialsPBR
{

extern const MaterialPBR *DIAMOND;
extern const MaterialPBR *GLASS;
extern const MaterialPBR *MATTE;
extern const MaterialPBR *MIRROR;
extern const MaterialPBR *METAL;
extern const MaterialPBR *TRANSLUCENT;

const MaterialPBR* material(std::string name);

}

#endif
