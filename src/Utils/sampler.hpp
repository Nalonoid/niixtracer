#ifndef __SAMPLER_HPP__
#define __SAMPLER_HPP__

#include <random>
#include "Math/math.hpp"

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

/* Compute a random point on a hemisphere, uniformly sampled
 * More information here :
 * http://www.rorydriscoll.com/2009/01/07/better-sampling/ */
Vec3d hemisphere_sample();

#endif
