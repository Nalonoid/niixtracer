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

/* Code from : https://users.cg.tuwien.ac.at/zsolnai/gfx/smallpaint/
 * Pseudo code may be found on wikipedia
 * https://en.wikipedia.org/wiki/Halton_sequence */
class Halton
{
public:
    void number(int i, int base) {
        double f = _inv_base = 1.0 / base;
        _value = 0.0;
        while (i > 0) {
            _value += f * (double)(i%base);
            i /= base;
            f *= _inv_base;
        }
    }

    void next() {
        double r = 1.0 - _value - 0.0000001;
        if (_inv_base < r) _value += _inv_base;
        else {
            double h = _inv_base;
            double hh;

            do
            {
                hh = h;
                h *= _inv_base;
            } while (h >= r);

            _value += hh + h - 1.0;
        }
    }

    double get() { return _value; }

private:
    double _value;
    double _inv_base;
};

extern Halton halton_sampler1;
extern Halton halton_sampler2;
extern Uniform uniform_sampler;

/* Compute a random point on a hemisphere, uniformly sampled
 * More information here :
 * http://www.rorydriscoll.com/2009/01/07/better-sampling/ */
Vec3d hemisphere_sample();

Vec3d rnd_dir_hemisphere(Vec3d normal);

#endif
