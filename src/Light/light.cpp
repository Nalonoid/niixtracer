#include "light.hpp"

Light::Light() :
    _position(Vec3d(0, 0, 0)), _color(Colors::WHITE) {}

Light::Light(const Vec3d &p, const Color& c) :
    _position(p), _color(c) {}

const Vec3d& Light::position() const
{
    return _position;
}

const Color& Light::color() const
{
    return _color;
}
