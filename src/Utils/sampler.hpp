#ifndef __SAMPLER_HPP__
#define __SAMPLER_HPP__

#include <random>
#include "Math/math.hpp"

template <template<class> class T, class U>
class Uniform
{
public:
    Uniform(U min = 0, U max = 1);
    U sample();

private:
    std::random_device _rnd_dv;
    std::mt19937 _gen;
    T<U> _distribution;
};

extern Uniform<std::uniform_real_distribution, double>  uniform_sampler_double;
extern Uniform<std::uniform_real_distribution, float>   uniform_sampler_float;
extern Uniform<std::uniform_real_distribution, double>  longitude_sampler;
extern Uniform<std::uniform_int_distribution, int>      wavelength_sampler;

/* Compute a random point on a hemisphere, uniformly sampled
 * More information here :
 * http://www.rorydriscoll.com/2009/01/07/better-sampling/ */
Vec3d hemisphere_sample();
Vec3d rnd_dir_hemisphere(Vec3d normal);

// Templated methods implementation must be visible for the translation unit to compile
template <template<class> class T, class U>
U Uniform<T, U>::sample()
{
    return _distribution(_gen);
}

#endif
