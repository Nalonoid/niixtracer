#include "light.hpp"

unsigned Light::_index = 0;

Light::Light() :
    Object(Space::ORIGIN), _color(Colors::WHITE)
{
    _index++;
}

Light::Light(const Vec3d &p, const Color& c, double emission) :
    Object(p), _color(c), _emission(emission)
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


double Light::emission() const
{
    return _emission;
}

double& Light::emission()
{
    return _emission;
}
