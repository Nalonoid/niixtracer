#ifndef __SPECTRUM_HPP__
#define __SPECTRUM_HPP__

#include <string>

#include "Math/math.hpp"

template <unsigned nb_samples = SPECTRAL_SAMPLES>
class Spectrum
{
public:
    Spectrum(float sample_value = 0.0);

    Spectrum operator+(const Spectrum &s);
    Spectrum operator-(const Spectrum &s);
    Spectrum operator*(const Spectrum &s);

    float& operator[](int index);

    virtual float power_at(const unsigned wavelength) const;

protected:
    float _samples[nb_samples];
};

template <unsigned nb_samples = SPECTRAL_SAMPLES>
class BlackBodySPD : public Spectrum<nb_samples>
{
public:
    // Default temperature is D65 illuminant's one : 6504°K
    BlackBodySPD(float temperature = 6504);

private:
    float _temperature;
};

template <unsigned nb_samples = SPECTRAL_SAMPLES>
class ConstantSPD : public Spectrum<nb_samples>
{
public:
    ConstantSPD(double sample_value = 0.0);
    float power_at(const unsigned wavelength) const override;

private:
    // Normalized power value of the constant distribution
    float _constant;
};

template <unsigned nb_samples = SPECTRAL_SAMPLES>
class NormalSPD : public Spectrum<nb_samples>
{
public:
    NormalSPD(unsigned peak = (MAX_WAVELENGTH - MIN_WAVELENGTH)/2,
              float sigma = 0.5f);

private:
    // The wavelength (nm) on which the distribution is centered (μ)
    unsigned    _peak;

    /* The standard deviation of the normal distribution
     * _sigma^2 is the variance */
    float       _sigma;
};

namespace Spectra
{

const ConstantSPD<>     CONSTANT_SPD    { 0.6f };

const NormalSPD<>       NORMAL_RED      { 685 };
const NormalSPD<>       NORMAL_ORANGE   { 595 };
const NormalSPD<>       NORMAL_YELLOW   { 580 };
const NormalSPD<>       NORMAL_GREEN    { 535 };
const NormalSPD<>       NORMAL_BLUE     { 475 };
const NormalSPD<>       NORMAL_PURPLE   { 415 };

const BlackBodySPD<>    BLACK_BODY_A    { 2856 }; // Incandescent / Tungstene
const BlackBodySPD<>    BLACK_BODY_D50  { 5003 }; // Horizon daylight
const BlackBodySPD<>    BLACK_BODY_D55  { 5503 }; // mid-morning / mid-afternoon daylight
const BlackBodySPD<>    BLACK_BODY_D65  { 6504 }; // Noon daylight
const BlackBodySPD<>    BLACK_BODY_D75  { 7504 }; // North sky daylight
const BlackBodySPD<>    BLACK_BODY_E    { 5454 }; // Equal energy

}

#include "spectrum.inl"

#endif
