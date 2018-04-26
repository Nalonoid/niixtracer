#include "intersection.hpp"

Intersection::Intersection() : _material(&Materials::DEFAULT) {}

unsigned Intersection::index() const
{
    return 0;
}

const Material* Intersection::material() const
{
    return _material;
}

void Intersection::set_material(Material *mat)
{
    _material = mat;
}


const Vec3d& Intersection::normal() const
{
    return _normal;
}

Vec3d& Intersection::normal()
{
    return _normal;
}
