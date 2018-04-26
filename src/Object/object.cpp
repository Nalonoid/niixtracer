#include "object.hpp"
#include "Raytracer/ray.hpp"

Object::Object(const Vec3d &position, double emission) :
    _position(position), _emission(emission) {}

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

double Object::emission() const
{
    return _emission;
}

double& Object::emission()
{
    return _emission;
}
