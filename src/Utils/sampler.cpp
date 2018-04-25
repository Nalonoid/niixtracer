#include <random>

#include "sampler.hpp"

Uniform::Uniform(double min, double max) :
    _gen(_rnd_dv()), _distribution(min, max) {}

double Uniform::sample()
{
    return _distribution(_gen);
}
