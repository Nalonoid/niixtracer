#include "matte.hpp"
#include "brdf.hpp"
#include "Utils/sampler.hpp"

Matte::Matte() : MaterialBRDF(BRDFs::LAMBERT) {}

Vec3d Matte::wi(const Vec3d&, Vec3d &normal) const
{
    // Cosine-weighted hemisphere sampling
    return Vec3d(rnd_dir_hemisphere(normal).normalized());
}
