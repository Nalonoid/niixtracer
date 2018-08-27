#include "fluorescence.hpp"

Fluorescence::Fluorescence(const Spectrum<> *absorption, unsigned stokes_shift)
    : _stokes_shift(stokes_shift), _absorption(absorption)
{
    /* Emission spectra are the same as absorption spectra
     * They're just shifted of stokes_shift nanometers and the tail of the
     * Cauchy distribution is on the opposite side (i.e. left) */

    _emission = CauchySkewed<>(_absorption->peak() + stokes_shift,
                               _absorption->width(),
                               -1.0f * _absorption->skewness());
}

float Fluorescence::emission(unsigned wavelength) const
{
    float emission = 0.0f;

    if (belongs_to_visible_spectrum(wavelength))
        emission = _emission.power_at(wavelength);

    return emission;
}

float Fluorescence::absorption(unsigned wavelength) const
{
    float absorption = 0.0f;

    if (belongs_to_visible_spectrum(wavelength))
        absorption = _absorption->power_at(wavelength);

    return absorption;
}

const Spectrum<>& Fluorescence::emission() const
{
    return _emission;
}

const Spectrum<>* Fluorescence::absorption() const
{
    return _absorption;
}

unsigned Fluorescence::stokes_shift() const
{
    return _stokes_shift;
}
