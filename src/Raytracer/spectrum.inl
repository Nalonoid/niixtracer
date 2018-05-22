#include <assert.h>

template <unsigned nb_samples>
Spectrum<nb_samples>::Spectrum(float sample_value)
{
    for (unsigned i {0}; i < nb_samples; ++i)
        _samples[i] = sample_value;
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
float Spectrum<nb_samples>::power_at(const unsigned wavelength) const
{
    int index { static_cast<int>(wavelength - MIN_WAVELENGTH) /
                static_cast<int>(nb_samples) };

    assert(index > 0);

    if (belongs_to_visible_spectrum(wavelength) && index < (int) nb_samples)
        return _samples[index];
    else
        return 0.0f;
}

// Black body emission Spectral Power Distribution
template <unsigned nb_samples>
BlackBodySPD<nb_samples>::BlackBodySPD(float temperature) :
    _temperature (temperature)
{
    unsigned range  { MAX_WAVELENGTH - MIN_WAVELENGTH   };
    unsigned step   { range / nb_samples                };

    for (unsigned i {0}; i < nb_samples; ++i)
    {
        // Plank's law
        float wavelength    { (MIN_WAVELENGTH + i * step) * 1e-9f       };
        float energy        { 3.74183e-16f * powf(wavelength, -5.0f)    };

        energy /= (exp(1.4388e-2f / (wavelength * _temperature)) - 1.0f);
        this->_samples[i] = energy;
    }
}

// Constant Spectral Power Distribution - can be used for diffuse materials
template <unsigned nb_samples>
ConstantSPD<nb_samples>::ConstantSPD(double sample_value) :
    Spectrum<>(sample_value) {}

template <unsigned nb_samples>
float ConstantSPD<nb_samples>::power_at(unsigned) const
{
    return _constant;
}

// Normal Spectral Power Distribution
template <unsigned nb_samples>
NormalSPD<nb_samples>::NormalSPD(unsigned peak, float sigma) :
    _peak(peak), _sigma(sigma)
{
    unsigned range  { MAX_WAVELENGTH - MIN_WAVELENGTH   };
    unsigned step   { range / nb_samples                };

    for (unsigned i {0}; i < nb_samples; ++i)
    {
        unsigned wavelength { MIN_WAVELENGTH + i * step };

        /* Normal distribution, with x the wavelength
         * f(x | μ, σ^2) = 1/(sqrt(2*PI*σ^2)) * exp(-(x-μ)^2 / 2*σ^2) */

        float sigma_2_2 { 2 * _sigma * _sigma                   };
        float norm_fact { 1.f/sqrtf(PI * PI * sigma_2_2 * 2.f)  };
        int deviation   { static_cast<int>(wavelength - _peak)  };
        deviation *= deviation;

        this->_samples[i] = norm_fact * exp(-deviation/sigma_2_2);
    }
}
