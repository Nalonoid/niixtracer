#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "Math/math.hpp"

namespace
{

float const EPSILON { 0.000000000001 };
float const PI      { 3.141592653589 };

unsigned const MIN_WAVELENGTH { 360 };
unsigned const MAX_WAVELENGTH { 830 };
unsigned const AVG_WAVELENGTH { (MIN_WAVELENGTH + MAX_WAVELENGTH) / 2};

unsigned const COLOR_MATCHING_FCTS_RES { 1 };
unsigned const SPECTRAL_SAMPLES { 1 + (MAX_WAVELENGTH - MIN_WAVELENGTH) /
            COLOR_MATCHING_FCTS_RES };

}

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

inline double schlick_approx(double n1, double n2, double cos_R, double sin2_T)
{
    double R { 0 };

    // If sin2(thetaT) > 1 we're in the case of Total Internal Reflection
    if (sin2_T > 1.0)
        R = 1.0;
    else
    {
        // Cosine of :  transmitted vector with n or reflected vector with n
        double used_cos { n1 > n2 ? sqrt(1 - sin2_T) : cos_R };
        double R0       { (n1 - n2) / (n1 + n2)              };
        double c        { 1 - used_cos                       };

        R0 *= R0;
        R = R0 + (1.0 - R0) * c * c * c * c * c;
    }

    return R;
}

#endif
