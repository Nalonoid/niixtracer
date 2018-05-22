#include <random>
#include <chrono>

#include "sampler.hpp"
#include "utils.hpp"

Uniform uniform_sampler;
Uniform longitude_sampler(0, 2*PI);
Uniform wavelength_sampler(MIN_WAVELENGTH, MAX_WAVELENGTH);

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

    double x { cos(theta) * r };
    double y { sin(theta) * r };

    return Vec3d(x, y, phi);
}

Vec3d rnd_dir_hemisphere(Vec3d normal)
{
    Vec3d x_axis, y_axis;
    Space::orthonormal_basis(normal, x_axis, y_axis);

    Vec3d hsphere_smpl { hemisphere_sample() };

    return Vec3d(hsphere_smpl.dot(x_axis),
                 hsphere_smpl.dot(y_axis),
                 hsphere_smpl.dot(normal));
}
