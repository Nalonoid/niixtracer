#include "shape.hpp"
#include "Raytracer/ray.hpp"
#include "Image/color.inl"

unsigned Shape::_index = 0;

Shape::Shape(const Vec3d &position, const Color &color) :
    Object(position, color)
{
    _index++;
}

Shape::Shape(const Color &color) :
    Object(Space::ORIGIN, color)
{
    _index++;
}

// Getters
unsigned Shape::index() const
{
    return _index;
}

// Methods
bool Shape::intersect(Ray&, double&)
{
    return 0.0;
}

const Vec3d Shape::normal_at(const Vec3d&)
{
    return Space::YAXIS;
}
