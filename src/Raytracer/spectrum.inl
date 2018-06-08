#include <assert.h>
#include <random>

#include "Utils/utils.hpp"
#include "Utils/color_matching.hpp"

template <unsigned nb_samples>
Spectrum<nb_samples>::Spectrum(float sample_value) :
    _xyz (Vec3f()), _rgb (Colors::BLACK)
{
    for (unsigned i {0}; i < nb_samples; ++i)
        _samples[i] = sample_value;

    // to_RGB() initializes _xyz and _rgb
    to_RGB();
}

template <unsigned nb_samples>
Spectrum<nb_samples> Spectrum<nb_samples>::operator+(const Spectrum &s)
{
    Spectrum<nb_samples> returned_spectrum = *this;

    for (unsigned i {0}; i < nb_samples; ++i)
        returned_spectrum[i] += s[i];

    return returned_spectrum;
}

template <unsigned nb_samples>
Spectrum<nb_samples> Spectrum<nb_samples>::operator-(const Spectrum &s)
{
    Spectrum<nb_samples> returned_spectrum = *this;

    for (unsigned i {0}; i < nb_samples; ++i)
        returned_spectrum[i] -= s[i];

    return returned_spectrum;
}

template <unsigned nb_samples>
Spectrum<nb_samples> Spectrum<nb_samples>::operator*(const Spectrum &s)
{
    Spectrum<nb_samples> returned_spectrum = *this;

    for (unsigned i {0}; i < nb_samples; ++i)
        returned_spectrum[i] *= s[i];

    return returned_spectrum;
}

template <unsigned nb_samples>
float& Spectrum<nb_samples>::operator[](int index)
{
    return _samples[index];
}

template <unsigned nb_samples>
float Spectrum<nb_samples>::operator[](int index) const
{
    return _samples[index];
}

template <unsigned nb_samples>
float Spectrum<nb_samples>::power_at(const unsigned wavelength) const
{
    int index { static_cast<int>(wavelength - MIN_WAVELENGTH) /
                static_cast<int>(nb_samples) };

    assert(index >= 0);

    if (belongs_to_visible_spectrum(wavelength) && index < (int) nb_samples)
        return _samples[index];
    else
        return 0.0f;
}

template <unsigned nb_samples>
void Spectrum<nb_samples>::to_XYZ()
{
    float luminance { 0.0f };

    for (unsigned i {0}; i < SPECTRAL_SAMPLES; ++i)
    {
        _xyz += (*this)[i] * CIE_cm_fcts[i];
        luminance += CIE_cm_fcts[i].y;
    }

    _xyz /= luminance;
}

// From PBRT - Second Edition by Matt Pharr & Greg Humphreys
template <unsigned nb_samples>
Color Spectrum<nb_samples>::to_RGB()
{
    to_XYZ();

    _rgb.r() = 3.240479f   * _xyz.x - 1.537150f * _xyz.y - 0.498535f * _xyz.z;
    _rgb.g() = -0.969256f  * _xyz.x + 1.875991f * _xyz.y + 0.041556f * _xyz.z;
    _rgb.b() = 0.055648f   * _xyz.x - 0.204043f * _xyz.y + 1.057311f * _xyz.z;

    return _rgb;
}

// Black body emission Spectral Power Distribution
template <unsigned nb_samples>
BlackBodySPD<nb_samples>::BlackBodySPD(float T) :
    _temperature (T)
{
    unsigned range  { MAX_WAVELENGTH - MIN_WAVELENGTH   };
    unsigned step   { range / nb_samples                };

    for (unsigned i {0}; i < nb_samples; ++i)
    {
        // Planck's law
        float h         { 6.626070040e-34   }; // Planck's constant
        float c         { 299792458.0       }; // speed of light in m.s-1
        float k         { 1.38064852e-23    }; // Boltzmann's constant

        float lambda    { (MIN_WAVELENGTH + i * step) * 1.0e-9f }; // wavelength in m
        float hc        { h * c                                 };

        double energy   { (2.0 * hc * c) * powf(lambda, -5.0)   };
        energy /= exp(hc/(lambda*k*T)) - 1.0;

        this->_samples[i] = energy; //* 1.0e-12;   // TODO remove Magic constants...
    }
}

// Constant Spectral Power Distribution - can be used for diffuse materials
template <unsigned nb_samples>
ConstantSPD<nb_samples>::ConstantSPD(float sample_value) :
    Spectrum<>(sample_value) {}

// Normal Spectral Power Distribution
template <unsigned nb_samples>
NormalSPD<nb_samples>::NormalSPD(unsigned peak, float sigma) :
    _peak(peak), _sigma(sigma)
{
    std::default_random_engine generator;
    std::normal_distribution<float> ndf(float(peak), sigma*nb_samples);

    unsigned range          { MAX_WAVELENGTH - MIN_WAVELENGTH   };
    unsigned experiments    { 100000                            };

    for (unsigned i {0}; i < experiments; ++i)
    {
        float nb { ndf(generator) };

        if (nb >= MIN_WAVELENGTH && nb <= MAX_WAVELENGTH)
        {
            float       norm_nb { float(nb - MIN_WAVELENGTH)/float(range)   };
            unsigned    index   { unsigned(norm_nb * nb_samples)            };

            this->_samples[index]++;
        }
    }

    for (unsigned i {0}; i < nb_samples; ++i)
        this->_samples[i] /= float(experiments);
}
