#include "object.hpp"

Object::Object(const Color &color) : _color(color) {}

const Color& Object::color() const
{
    return _color;
}

Color &Object::color()
{
    return _color;
}

// Methods

double Object::intersection(Ray)
{
    return 0.0;
}

const Vec3d& Object::normal_at(Vec3d)
{
    return Space::YAXIS;
}
