#include "object.hpp"
#include "Raytracer/ray.hpp"

Object::Object(const Vec3d &position) :
    _position(position) {}

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
