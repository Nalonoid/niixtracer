#include "fluorescence.hpp"

Fluorescence::Fluorescence(const Spectrum<> *absorption, unsigned stokes_shift)
    : _stokes_shift(stokes_shift), _absoprtion(absorption)
{
    /* Emission spectra are the same as absorption spectra
     * They're just shifted of stokes_shift nanometers and the tail of the
     * Cauchy distribution is on the opposite side (i.e. left) */

    _emission = CauchySkewed<>(_absoprtion->peak() + stokes_shift,
                               _absoprtion->width(),
                               -1.0f * _absoprtion->skewness());
}

float Fluorescence::emission(unsigned wavelength) const
{
    float emission = 0.0f;

    if (belongs_to_visible_spectrum(wavelength))
        emission = _absoprtion->power_at(wavelength - _stokes_shift) != 0.0f ?
                    _emission.power_at(wavelength) : 0.0f;

    return emission;
}

const Spectrum<>& Fluorescence::emission() const
{
    return _emission;
}

const Spectrum<>* Fluorescence::absorption() const
{
    return _absoprtion;
}
