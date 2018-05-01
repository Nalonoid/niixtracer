#include <random>

#include "sampler.hpp"

Uniform::Uniform(double min, double max) :
    _gen(_rnd_dv()), _distribution(min, max) {}

double Uniform::sample()
{
    return _distribution(_gen);
}

Vec3d hemisphere_sample()
{
    Uniform sampler;

    double u { sampler.sample() };
    double v { sampler.sample() };

    // Compute a random point on a hemisphere
    double r    { sqrt(1 - u*u) };
    double phi  { 2 * PI * v    };

    double x { cos(phi) * r };
    double y { sin(phi) * r };

    return Vec3d(x, y, u);
}
