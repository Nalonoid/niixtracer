#include "light.hpp"

unsigned Light::_index = 0;

Light::Light(double emission, const Vec3d &p, const Color& c) :
    Object(p), _color(c), _emission_spectrum(new ConstantSPD<>(emission))
{
    _index++;
}

Light::Light(const Spectrum<> *s, const Vec3d &p, const Color& c) :
    Object(p), _color(c), _emission_spectrum(s)
{
    _index++;
}

// Getters
unsigned Light::index() const
{
    return _index;
}

const Color& Light::color() const
{
    return _color;
}

double Light::brightness() const
{
    return _color.brightness();
}


double Light::emission(unsigned wavelength) const
{
    return _emission_spectrum->power_at(wavelength);
}
