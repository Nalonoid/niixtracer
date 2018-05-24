#include "intersection.hpp"

Intersection::Intersection() : _shape (nullptr) {}

unsigned Intersection::index() const
{
    return 0;
}

const Shape* Intersection::shape() const
{
    return _shape;
}

void Intersection::set_shape(Shape* s)
{
    _shape = s;
}

const Vec3d& Intersection::normal() const
{
    return _normal;
}

Vec3d& Intersection::normal()
{
    return _normal;
}
