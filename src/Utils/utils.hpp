#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "Math/math.hpp"
#include "Image/color.hpp"

namespace
{

float const EPSILON { 0.000000000001 };
float const PI      { 3.141592653589 };

unsigned const MIN_WAVELENGTH { 360 };
unsigned const MAX_WAVELENGTH { 780 };

unsigned const MIN_VISIBLE_WAVELENGTH { 400 };
unsigned const MAX_VISIBLE_WAVELENGTH { 740 };

unsigned const AVG_WAVELENGTH { (MIN_WAVELENGTH + MAX_WAVELENGTH) / 2 };

unsigned const SPECTRAL_RES     { 5 };
unsigned const SPECTRAL_SAMPLES { 1 + (MAX_WAVELENGTH - MIN_WAVELENGTH) /
            SPECTRAL_RES };

unsigned const SPECTRAL_VISIBLE_SAMPLES
{ 1 + (MAX_VISIBLE_WAVELENGTH - MIN_VISIBLE_WAVELENGTH) / SPECTRAL_RES };

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
    return wavelength >= MIN_VISIBLE_WAVELENGTH
            && wavelength <= MAX_VISIBLE_WAVELENGTH;
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

// Applies the sRGB gamma correction to the component
inline static Color correct_gamma(Color c)
{
    double r { c.r() };
    double g { c.g() };
    double b { c.b() };

    float ratio { 1.0f / 2.4f };

    if (r <= 0.0031308f)
        r = 12.92f * r;
    else
        r = 1.055f * pow(r, ratio) - 0.055f;

    if (g <= 0.0031308f)
        g = 12.92f * g;
    else
        g = 1.055f * pow(g, ratio) - 0.055f;

    if (b <= 0.0031308f)
        b = 12.92f * b;
    else
        b = 1.055f * pow(b, ratio) - 0.055f;

    return Color(r, g, b).clamp();
}

#endif
