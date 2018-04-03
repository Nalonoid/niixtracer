#include "object.hpp"
#include "Raytracer/ray.hpp"

Object::Object(const Vec3d &position, const Color &color) :
    _position(position), _color(color) {}

Object::~Object() {}

// Getters
const Vec3d& Object::position() const
{
    return _position;
}

Vec3d& Object::position()
{
    return _position;
}

const Color& Object::color() const
{
    return _color;
}

Color &Object::color()
{
    return _color;
}
