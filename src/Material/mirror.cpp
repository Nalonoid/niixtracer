#include "mirror.hpp"
#include "brdf.hpp"
#include "Utils/sampler.hpp"

Mirror::Mirror(float glossiness) : MaterialPBR(BRDFs::IDEAL_SPECULAR),
    _roughness (glossiness) {}

float Mirror::roughness() const
{
    return _roughness;
}

Vec3d Mirror::wi(const Vec3d &wo, Vec3d &normal) const
{
    double u { uniform_sampler.sample() };

    if (u <= _roughness)    // Reflection is in the ideal specular direction
        return wo.reflect(normal);
    else                    // We took a random diffuse sample over the hmsphere
        return Vec3d(rnd_dir_hemisphere(normal).normalized());
}
