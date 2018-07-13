#include <assert.h>
#include <random>

#include "Utils/utils.hpp"
#include "Utils/color_matching.hpp"

template <unsigned nb_samples>
Spectrum<nb_samples>::Spectrum(float sample_value) :
    _nb_samples(nb_samples)
{
    for (unsigned i {0}; i < nb_samples; ++i)
        _samples[i] = sample_value;

    _xyz = Vec3f(0.0f, 0.0f, 0.0f);
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
                static_cast<int>(SPECTRAL_RES) };

    if (index >= 0 && index < (int) nb_samples)
        return _samples[index];
    else
        return 0.0f;
}

template <unsigned nb_samples>
unsigned Spectrum<nb_samples>::peak() const
{
    unsigned index  { 0     };
    float max       { 0.0f  };

    for (unsigned i {0}; i < nb_samples; ++i)
    {
        if (_samples[i] > max)
        {
            max = _samples[i];
            index = i;
        }
    }

    return index * SPECTRAL_RES;
}

template <unsigned nb_samples>
float Spectrum<nb_samples>::width() const
{
    return 0.0f;
}

template <unsigned nb_samples>
float Spectrum<nb_samples>::skewness() const
{
    return 0.0f;
}

template <unsigned nb_samples>
const float* Spectrum<nb_samples>::samples() const
{
    return _samples;
}

template <unsigned nb_samples>
unsigned Spectrum<nb_samples>::n_samples() const
{
    return _nb_samples;
}

template <unsigned nb_samples>
template <unsigned nbs>
void Spectrum<nb_samples>::to_XYZ(const Spectrum<nbs> *illuminant_SPD)
{
    float luminance { 0.0f };

    for (unsigned i {0}; i < nb_samples; ++i)
    {
        _xyz += (*this)[i] * CIE_cm_fcts[i*SPECTRAL_RES];
        luminance += (*illuminant_SPD)[i] * CIE_cm_fcts[i*SPECTRAL_RES].y;
    }

    _xyz *= (MAX_VISIBLE_WAVELENGTH - MIN_VISIBLE_WAVELENGTH) / luminance;
}

// From PBRT - Second Edition by Matt Pharr & Greg Humphreys
template <unsigned nb_samples>
template <unsigned nbs>
Color Spectrum<nb_samples>::to_RGB(const Spectrum<nbs> *illuminant_SPD)
{
    to_XYZ(illuminant_SPD);

    Color rgb;

    rgb.r() += 3.240479f   * _xyz.x - 1.537150f * _xyz.y - 0.498535f * _xyz.z;
    rgb.g() += -0.969256f  * _xyz.x + 1.875991f * _xyz.y + 0.041556f * _xyz.z;
    rgb.b() += 0.055648f   * _xyz.x - 0.204043f * _xyz.y + 1.057311f * _xyz.z;

    return rgb;
}

// Black body emission Spectral Power Distribution
template <unsigned nb_samples>
BlackBodySPD<nb_samples>::BlackBodySPD(float T) :
    _temperature (T)
{
    unsigned res { SPECTRAL_RES };

    for (unsigned i {0}; i < nb_samples; ++i)
    {
        // Planck's law
        double h        { 6.626070040e-34   }; // Planck's constant
        double c        { 299792458.0       }; // speed of light in m.s-1
        double k        { 1.38064852e-23    }; // Boltzmann's constant

        double lambda   { (MIN_WAVELENGTH + i * res) * 1.0e-9   }; // wavelength in m
        double hc       { h * c                                 };
//        double v        { c / lambda                            }; // frequency

//        double energy   { 2.0 * h * v * v * v };
//        energy /= c * c * (exp(h*v/(k*T)) - 1.0);

        double energy   { 2.0 * hc * c * pow(lambda, -5.0) };
        energy /= (exp(hc/(lambda*k*T)) - 1.0);

        this->_samples[i] = energy;
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

    unsigned range          { MAX_WAVELENGTH - MIN_WAVELENGTH       };
    unsigned experiments    { 100000                                };

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

    unsigned peak_index { (peak - MIN_WAVELENGTH)/SPECTRAL_RES      };
    float peak_energy   { this->_samples[peak_index]/experiments    };

    for (unsigned i {0}; i < nb_samples; ++i)
    {
        this->_samples[i] /= experiments;
        this->_samples[i] /= peak_energy;
    }
}

template <unsigned nb_samples>
unsigned NormalSPD<nb_samples>::peak() const
{
    return _peak;
}

template <unsigned nb_samples>
CauchySkewed<nb_samples>::CauchySkewed(unsigned x0, float w, float s) :
    _peak(x0), _width(w), _skewness(s)
{
    for (unsigned i {0}; i < nb_samples; ++i)
    {
        unsigned x { MIN_WAVELENGTH + i * SPECTRAL_RES };   // wavelength in nm

        float x_min_x0      { float(x - x0)                         };
        float first_deno    { PI * (w * w + x_min_x0 * x_min_x0)    };
        float first_term    { w / first_deno                        };
        float secnd_term    { std::atan(s * x_min_x0 / w)           };
        secnd_term = 1.0f/PI * secnd_term + 0.5f;

        this->_samples[i] = first_term * secnd_term;
    }

    unsigned peak_index { (x0 - MIN_WAVELENGTH)/SPECTRAL_RES    };
    float peak_energy   { this->_samples[peak_index]            };

    for (unsigned i {0}; i < nb_samples; ++i)
        this->_samples[i] /= peak_energy;
}

template <unsigned nb_samples>
unsigned CauchySkewed<nb_samples>::peak() const
{
    return _peak;
}

template <unsigned nb_samples>
float CauchySkewed<nb_samples>::width() const
{
    return _width;
}

template <unsigned nb_samples>
float CauchySkewed<nb_samples>::skewness() const
{
    return _skewness;
}
