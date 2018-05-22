#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "Math/math.hpp"

inline double clamp(double value, double min, double max)
{
    return value < min ?
                min : value > max ?
                    max : value;
}

inline double max(double a, double b)
{
    return a > b ? a : b;
}

inline double min(double a, double b)
{
    return a < b ? a : b;
}

inline bool belongs_to_visible_spectrum(unsigned wavelength)
{
    return wavelength >= MIN_WAVELENGTH && wavelength <= MAX_WAVELENGTH;
}

#endif
