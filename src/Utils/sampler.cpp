#include <random>
#include <chrono>

#include "sampler.hpp"

Uniform<std::uniform_real_distribution, double> uniform_sampler_double;
Uniform<std::uniform_real_distribution, float>  uniform_sampler_float;
Uniform<std::uniform_real_distribution, double> longitude_sampler(0, 2*PI);
Uniform<std::uniform_int_distribution, int>     wavelength_sampler(MIN_WAVELENGTH, MAX_WAVELENGTH);

template <template<class> class T, class U>
Uniform<T, U>::Uniform(U min, U max) :
    _gen(_rnd_dv()), _distribution(min, max) {}

Vec3d hemisphere_sample()
{
    double u { uniform_sampler_double.sample()  };
    double v { longitude_sampler.sample()       };

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
