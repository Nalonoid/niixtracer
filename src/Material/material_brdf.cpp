#include "material_brdf.hpp"
#include "matte.hpp"
#include "glass.hpp"
#include "mirror.hpp"
#include "brdf.hpp"


MaterialBRDF::MaterialBRDF() :
    _brdf(nullptr) {}

MaterialBRDF::MaterialBRDF(const BRDF *brdf) :
    _brdf(brdf) {}

const BRDF* MaterialBRDF::brdf() const
{
    return _brdf;
}

float MaterialBRDF::reflectance(const Vec3d &wi, const Vec3d &wo,
                                const Intersection &i,
                                const unsigned wavelength) const
{
    return brdf()->evaluate(wi, wo, i, wavelength);
}

namespace MaterialsBRDF
{

const MaterialBRDF *MATTE   { new Matte()     };
const MaterialBRDF *GLASS   { new Glass()     };
const MaterialBRDF *MIRROR  { new Mirror()    };

}
