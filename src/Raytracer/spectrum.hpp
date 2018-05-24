#ifndef __SPECTRUM_HPP__
#define __SPECTRUM_HPP__

#include <string>

#include "Math/math.hpp"

template <unsigned nb_samples = SPECTRAL_SAMPLES>
class Spectrum
{
public:
    Spectrum(float sample_value = 0.0f);

    Spectrum operator+(const Spectrum &s);
    Spectrum operator-(const Spectrum &s);
    Spectrum operator*(const Spectrum &s);

    float& operator[](int index);

    float power_at(const unsigned wavelength) const;

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
    ConstantSPD(float sample_value = 0.0);

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

//namespace Spectra
//{

//const ConstantSPD<> CONSTANT_SPD { ConstantSPD<>(0.6f) };

//const NormalSPD<>       NORMAL_RED      { NormalSPD<>(685) };
//const NormalSPD<>       NORMAL_ORANGE   { NormalSPD<>(595) };
//const NormalSPD<>       NORMAL_YELLOW   { NormalSPD<>(580) };
//const NormalSPD<>       NORMAL_GREEN    { NormalSPD<>(535) };
//const NormalSPD<>       NORMAL_BLUE     { NormalSPD<>(475) };
//const NormalSPD<>       NORMAL_PURPLE   { NormalSPD<>(415) };

//const BlackBodySPD<>    BLACK_BODY_A    { BlackBodySPD<>(2856) }; // Incandescent / Tungstene
//const BlackBodySPD<>    BLACK_BODY_D50  { BlackBodySPD<>(5003) }; // Horizon daylight
//const BlackBodySPD<>    BLACK_BODY_D55  { BlackBodySPD<>(5503) }; // mid-morning / mid-afternoon daylight
//const BlackBodySPD<>    BLACK_BODY_D65  { BlackBodySPD<>(6504) }; // Noon daylight
//const BlackBodySPD<>    BLACK_BODY_D75  { BlackBodySPD<>(7504) }; // North sky daylight

//const BlackBodySPD<>    BLACK_BODY_E    { BlackBodySPD<>(5454) }; // Equal energy

//}

#include "spectrum.inl"

#endif
