#include "intersection.hpp"

Intersection::Intersection() : _exists(false), _material(&Materials::DEFAULT) {}

unsigned Intersection::index() const
{
    return 0;
}

bool Intersection::exists() const
{
    return _exists;
}

bool& Intersection::exists()
{
    return _exists;
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
