#include "material.hpp"

MATERIAL_TYPE Material::type() const
{
    return _type;
}

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

std::string Material::name() const
{
    return _name;
}

namespace Materials {

    const Material& material(std::string name)
    {
        if (name == "default")
            return Materials::DEFAULT;

        if (name == "plastic")
            return Materials::PLASTIC;

        if (name == "metal")
            return Materials::METAL;

        if (name == "glass")
            return Materials::GLASS;

        if (name == "diffuse")
            return Materials::DIFFUSE;

        if (name == "glossy")
            return Materials::GLOSSY;

        if (name == "translucent")
            return Materials::TRANSLUCENT;

        return Materials::DEFAULT;
    }
}
