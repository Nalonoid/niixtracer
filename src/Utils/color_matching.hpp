#ifndef __COLOR_MATCHING_HPP__
#define __COLOR_MATCHING_HPP__

#include "Utils/utils.hpp"
#include "Raytracer/spectrum.hpp"

class ColorMatching
{
public:
    static float x_function(unsigned wavelength);
    static float y_function(unsigned wavelength);
    static float z_function(unsigned wavelength);

    static Vec3f from_SPD_to_XYZ(const Spectrum<> *spd, SPD_TYPE type);

private:
    static Vec3f _color_matching_fcts[SPECTRAL_SAMPLES];
};

#endif
