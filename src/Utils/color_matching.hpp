#ifndef __COLOR_MATCHING_HPP__
#define __COLOR_MATCHING_HPP__

#include "Utils/utils.hpp"

class ColorMatching
{
public:
    ColorMatching();

    static float x_function(unsigned wavelength);
    static float y_function(unsigned wavelength);
    static float z_function(unsigned wavelength);

    // Operators
    Vec3f& operator[](int index);
    const Vec3f& operator[](int index) const;

private:
    static Vec3f _color_matching_fcts[SPECTRAL_SAMPLES];
};

// CIE 1931 Color Matching Functions
extern ColorMatching CIE_cm_fcts;

#endif
