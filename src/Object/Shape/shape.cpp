#include "shape.hpp"
#include "Raytracer/ray.hpp"
#include "Raytracer/intersection.hpp"

unsigned Shape::_index = 0;

Shape::Shape(const Vec3d &position, const Material &material, double emission) :
    Object(position, emission), _material(material)
{
    _index++;
}

Shape::Shape(const Material &material) :
    Object(Space::ORIGIN), _material(material)
{
    _index++;
}

// Getters
unsigned Shape::index() const
{
    return _index;
}

const Material& Shape::material() const
{
    return _material;
}

Material &Shape::material()
{
    return _material;
}

// Methods
bool Shape::intersect(Ray &r, double t)
{   
    if (t > 0 && t < r.dist_max())
    {
        r.dist_max() = t;
        r.intersection().position() = r.origin() + (t * r.direction());
        r.intersection().set_material(&_material);

        return true;
    }

    return false;
}

const Vec3d Shape::normal_at(const Vec3d&)
{
    return Space::YAXIS;
}
