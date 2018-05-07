#include <random>

#include "sampler.hpp"

Uniform uniform_sampler;

Halton halton_sampler1;
Halton halton_sampler2;
Halton halton_sampler3;

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
