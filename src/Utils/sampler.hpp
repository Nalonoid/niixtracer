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

// Class for generating the Halton low-discrepancy series for Quasi
// Monte Carlo integration.
class Halton {
    double value, inv_base;
public:
    void number(int i, int base) {
        double f = inv_base = 1.0 / base;
        value = 0.0;
        while (i > 0) {
            value += f * (double)(i%base);
            i /= base;
            f *= inv_base;
        }
    }
    void next() {
        double r = 1.0 - value - 0.0000001;
        if (inv_base < r) value += inv_base;
        else {
            double h = inv_base, hh;
            do { hh = h; h *= inv_base; } while (h >= r);
            value += hh + h - 1.0;
        }
    }
    double get() { return value; }
};

extern Halton halton_sampler1;
extern Halton halton_sampler2;
extern Halton halton_sampler3;
extern Uniform uniform_sampler;

/* Compute a random point on a hemisphere, uniformly sampled
 * More information here :
 * http://www.rorydriscoll.com/2009/01/07/better-sampling/ */
Vec3d hemisphere_sample();

#endif
