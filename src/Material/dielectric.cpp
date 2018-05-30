#include "dielectric.hpp"
#include "brdf.hpp"
#include "Utils/sampler.hpp"
#include "Utils/utils.hpp"

Dielectric::Dielectric(float ior, float roughness, std::string name) :
    MaterialPBR(&BRDFs::IDEAL_REFRACTION, name), _ior (ior),
    _roughness (roughness) {}

Vec3d Dielectric::wi(const Vec3d &wo, Vec3d &normal) const
{
    float n1 { 1.0f };
    float n2 { _ior };

    if (normal.dot(wo) > 0.0)  // Ray's inside the medium, going outside
    {
        normal = normal.negative();

        // swap n1 and n2
        n1 = n1 + n2;
        n2 = n1 - n2;
        n1 = n1 - n2;
    }

    float   n       { n1 / n2               };
    double  cos_R   { -wo.dot(normal)       };
    double  sin2_T  { n*n*(1 - cos_R*cos_R) };

    double R        { schlick_approx(n1, n2, cos_R, sin2_T) };
    double u        { uniform_sampler_double.sample()       };

    if (u > R)
        // Refraction
        return Vec3d(n * wo + (n * cos_R - sqrt(1.0 - sin2_T)) * normal);
    else
    {
        float v { uniform_sampler_float.sample() };

        if (v > _roughness)
            return wo.reflect(normal);
        else
            return Vec3d(rnd_dir_hemisphere(normal).normalized());

    }
}

float Dielectric::roughness() const
{
    return _roughness;
}
