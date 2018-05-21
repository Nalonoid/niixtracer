#include "spectrum.hpp"

template <unsigned nb_samples>
Spectrum<nb_samples>::Spectrum(double sample_value)
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
double& Spectrum<nb_samples>::operator[](int index)
{
    return _samples[index];
}
