#include "shape.hpp"
#include "Raytracer/ray.hpp"
#include "Raytracer/intersection.hpp"
#include "Image/color.inl"

unsigned Shape::_index = 0;

Shape::Shape(const Vec3d &position, const Color &color) :
    Object(position), _color(color)
{
    _index++;
}

Shape::Shape(const Color &color) :
    Object(Space::ORIGIN), _color(color)
{
    _index++;
}

// Getters
unsigned Shape::index() const
{
    return _index;
}

const Color& Shape::color() const
{
    return _color;
}

Color &Shape::color()
{
    return _color;
}


// Methods
bool Shape::intersect(Ray &r, double &t)
{
    if (t >= 0 && t < r.dist_max())
    {
        r.dist_max() = t;
        r.intersection().position() = r.origin() + (t * r.direction());
        r.intersection().ks() = _color;
        r.intersection().kd() = _color;
        return true;
    }

    return false;
}

const Vec3d Shape::normal_at(const Vec3d&)
{
    return Space::YAXIS;
}
