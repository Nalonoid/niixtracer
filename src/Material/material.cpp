#include "material.hpp"

MATERIAL_TYPE Material::type() const
{
    return _type;
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

namespace Materials
{

const Material* material(std::string name)
{
    if (name == "default")
        return &Materials::DEFAULT;

    if (name == "plastic")
        return &Materials::PLASTIC;

    if (name == "metal")
        return &Materials::METAL;

    if (name == "glass")
        return &Materials::GLASS;

    if (name == "diffuse")
        return &Materials::DIFFUSE;

    if (name == "diamond")
        return &Materials::DIAMOND;

    if (name == "translucent")
        return &Materials::TRANSLUCENT;

    if (name == "mirror")
        return &Materials::MIRROR;

    std::cerr << "Wrong material name" << std::endl;
    return nullptr;
}

}
