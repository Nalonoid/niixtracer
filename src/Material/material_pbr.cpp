#include "material_pbr.hpp"
#include "matte.hpp"
#include "glass.hpp"
#include "mirror.hpp"
#include "brdf.hpp"

MaterialPBR::MaterialPBR(const BRDF &brdf) :
    _brdf(brdf) {}

const std::string& MaterialPBR::name() const
{
    return _name;
}

const BRDF& MaterialPBR::brdf() const
{
    return _brdf;
}

float MaterialPBR::roughness() const
{
    return 0.0f;
}

float MaterialPBR::reflectance(const Vec3d &wi, const Vec3d &wo,
                                const Intersection &i,
                                const unsigned wavelength) const
{
    return _brdf.evaluate(wi, wo, i, wavelength);
}

namespace MaterialsPBR
{

const MaterialPBR *MATTE   { new Matte()    };
const MaterialPBR *GLASS   { new Glass()    };
const MaterialPBR *MIRROR  { new Mirror()   };

const MaterialPBR* material(std::string name)
{
    if (name == "matte")
        return MaterialsPBR::MATTE;

    if (name == "mirror")
        return MaterialsPBR::MIRROR;

    if (name == "glass")
        return MaterialsPBR::GLASS;

    return nullptr;
}


}
