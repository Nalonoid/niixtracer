#include <random>

#include "sampler.hpp"

Uniform uniform_sampler;

Halton halton_sampler1;
Halton halton_sampler2;

Uniform::Uniform(double min, double max) :
    _gen(_rnd_dv()), _distribution(min, max) {}

double Uniform::sample()
{
    return _distribution(_gen);
}

Vec3d hemisphere_sample()
{
    double u { halton_sampler1.get() };
    double v { halton_sampler2.get() };

    halton_sampler1.next();
    halton_sampler2.next();

    // Compute a random point on a hemisphere
    double r    { sqrt(1 - u*u) };
    double phi  { 2 * PI * v    };

    double x { cos(phi) * r };
    double y { sin(phi) * r };

    return Vec3d(x, y, u);
}

Vec3d rnd_dir_hemisphere(Vec3d normal)
{
    Vec3d x_axis, y_axis;
    Space::orthonormal_basis(normal, x_axis, y_axis);

    Vec3d hsphere_smpl  { hemisphere_sample() };

    return Vec3d(Vec3d(x_axis.x, y_axis.x, normal.x).dot(hsphere_smpl),
                 Vec3d(x_axis.y, y_axis.y, normal.y).dot(hsphere_smpl),
                 Vec3d(x_axis.z, y_axis.z, normal.z).dot(hsphere_smpl));
}
