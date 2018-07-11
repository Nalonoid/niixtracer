#ifndef __FLUORESCENCE_HPP__
#define __FLUORESCENCE_HPP__

#include "Raytracer/spectrum.hpp"

class Fluorescence
{
public:
    Fluorescence(const Spectrum<> *absorption, unsigned stokes_shift);

    float emission(unsigned wavelength) const;

    // Getters
    const Spectrum<>* absorption() const;
    const Spectrum<> &emission() const;

private:
    unsigned _stokes_shift;
    const Spectrum<> *_absoprtion;
    Spectrum<> _emission;
};

#endif
