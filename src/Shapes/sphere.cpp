#include "sphere.hpp"

Sphere::Sphere() : Object(), _position(Space::ORIGIN), _radius(1.0) {}

Sphere::Sphere(const Vec3d &position, double radius) :
    _position(position), _radius(radius) {}

Sphere::Sphere(const Vec3d &position, double radius, const Color &color) :
    Object(color), _position(position), _radius(radius) {}

// Getters
const Vec3d &Sphere::position() const
{
    return _position;
}

Vec3d &Sphere::position()
{
    return _position;
}

double Sphere::radius() const
{
    return _radius;
}

double& Sphere::radius()
{
    return _radius;
}

// Methods
double Sphere::intersection(Ray)
{
    return 0.0;
}

const Vec3d& Sphere::normal_at(Vec3d)
{
    return Space::YAXIS;
}
