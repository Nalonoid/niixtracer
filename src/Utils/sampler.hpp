#ifndef __SAMPLER_HPP__
#define __SAMPLER_HPP__

#include <random>

class Uniform
{
public:
    Uniform(double min = 0.0, double max = 1.0);
    double sample();

private:
    std::random_device _rnd_dv;
    std::mt19937 _gen;
    std::uniform_real_distribution<double> _distribution;
};

#endif
