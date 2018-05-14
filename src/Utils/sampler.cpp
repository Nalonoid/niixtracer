#include <random>
#include <chrono>

#include "sampler.hpp"
#include "utils.hpp"

Uniform uniform_sampler;
Uniform longitude_sampler(0, 2*PI);
Uniform wavelength_sampler(430, 770);

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
    double u { uniform_sampler.sample()     };
    double v { longitude_sampler.sample()   };

    // Compute a random point on a hemisphere
    double phi      { 2.0 * u - 1.0     };
    double theta    { v                 };
    double r        { sqrt(1 - phi*phi) };

    double x { cos(theta) * r   };
    double y { sin(theta) * r   };

    return Vec3d(x, y, phi);
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
