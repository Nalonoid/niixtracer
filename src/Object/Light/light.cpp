#include "light.hpp"

unsigned Light::_index = 0;

Light::Light() :
    Object(Space::ORIGIN, Colors::WHITE)
{
    _index++;
}

Light::Light(const Vec3d &p, const Color& c) :
    Object(p, c)
{
    _index++;
}

// Getters
unsigned Light::index() const
{
    return _index;
}
