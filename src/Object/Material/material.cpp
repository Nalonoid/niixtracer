#include "material.hpp"

const Color& Material::color() const
{
    return _color;
}

Color& Material::color()
{
    return _color;
}

double Material::shininess() const
{
    return _shininess;
}

double Material::reflection() const
{
    return _reflection;
}

double Material::refraction() const
{
    return _refraction;
}
