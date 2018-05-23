#include "mirror.hpp"
#include "brdf.hpp"
#include "Utils/sampler.hpp"

Mirror::Mirror(float glossiness) : MaterialBRDF(BRDFs::IDEAL_SPECULAR),
    _roughness (glossiness) {}

Vec3d Mirror::wi(const Vec3d &wo, Vec3d &normal) const
{
    double u { uniform_sampler.sample() };

    if (u <= _roughness)
        return wo.reflect(normal);
    else
        return Vec3d(rnd_dir_hemisphere(normal).normalized());
}
