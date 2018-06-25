#ifndef __SPECTRUM_HPP__
#define __SPECTRUM_HPP__

#include <string>

#include "Utils/utils.hpp"
#include "Image/color.hpp"

template <unsigned nb_samples = SPECTRAL_SAMPLES>
class Spectrum
{
public:
    Spectrum(float sample_value = 0.0f);

    Spectrum operator+(const Spectrum &s);
    Spectrum operator-(const Spectrum &s);
    Spectrum operator*(const Spectrum &s);

    float& operator[](int index);
    float  operator[](int index) const;

    float power_at(const unsigned wavelength) const;

    const float* samples() const;
    unsigned n_samples() const;

    virtual void to_XYZ(const Spectrum<nb_samples> *illuminant_SPD);
    virtual Color to_RGB(const Spectrum<nb_samples> *illuminant_SPD);

protected:
    float       _samples[nb_samples];     // Spectral Power Distribution
    unsigned    _nb_samples;
    Vec3f       _xyz;
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
    ConstantSPD(float sample_value = 1.0);
};

template <unsigned nb_samples = SPECTRAL_SAMPLES>
class NormalSPD : public Spectrum<nb_samples>
{
public:
    NormalSPD(unsigned peak = (MAX_WAVELENGTH - MIN_WAVELENGTH)/2,
              float sigma = 0.5f);

private:
    // The wavelength (nm) on which the distribution is centered (μ)
    unsigned _peak;

    /* The standard deviation of the normal distribution
     * _sigma^2 is the variance */
    float _sigma;
};

namespace Spectra
{

extern const ConstantSPD<>     *CONSTANT_SPD;

extern const NormalSPD<>       *NORMAL_RED;
extern const NormalSPD<>       *NORMAL_ORANGE;
extern const NormalSPD<>       *NORMAL_YELLOW;
extern const NormalSPD<>       *NORMAL_GREEN;
extern const NormalSPD<>       *NORMAL_BLUE;
extern const NormalSPD<>       *NORMAL_PURPLE;

extern const BlackBodySPD<>    *BLACK_BODY_A;
extern const BlackBodySPD<>    *BLACK_BODY_D50;
extern const BlackBodySPD<>    *BLACK_BODY_D55;
extern const BlackBodySPD<>    *BLACK_BODY_D65;
extern const BlackBodySPD<>    *BLACK_BODY_D75;

extern const BlackBodySPD<>    *BLACK_BODY_E;

const Spectrum<>* spectrum(std::string name);

}

#include "spectrum.inl"

#endif
