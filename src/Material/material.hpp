#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "Image/color.hpp"

enum class MATERIAL_TYPE { DIFFUSE, SPECULAR, REFRACTIVE };

typedef struct Material_s
{
    // Getters
    MATERIAL_TYPE   type()       const;
    const Color&    color()      const;
    double          shininess()  const;
    double          reflection() const;
    double          refraction() const;

    Color& color();

    std::string name() const;

    // Members
    std::string     _name;
    Color           _color;
    double          _shininess;
    double          _reflection;    // Can be seen as albedo (how much light is reflected)
    double          _refraction;
    MATERIAL_TYPE   _type;

    Material_s(Color c) :
        _name("default"), _color(c), _shininess(250), _reflection(0.2),
        _refraction(1.5), _type(MATERIAL_TYPE::DIFFUSE) {}

    Material_s(std::string name, Color c, double sh, double refl, double refr) :
        _name(name), _color(c), _shininess(sh), _reflection(refl),
        _refraction(refr)
    {
        _type = _refraction > 0.0 ? MATERIAL_TYPE::REFRACTIVE :
                _reflection > 0.0 ? MATERIAL_TYPE::SPECULAR :
                                    MATERIAL_TYPE::DIFFUSE;
    }
} Material;

namespace Materials
{

const Material DEFAULT       { "default",     Colors::MAROON, 200.0, 0.01, 0.000 };
const Material PLASTIC       { "plastic",     Colors::CYAN,   200.0, 0.30, 0.000 };
const Material METAL         { "metal",       Colors::CYAN,    10.0, 0.70, 0.000 };
const Material GLASS         { "glass",       Colors::CYAN,    50.0, 0.90, 1.540 };
const Material DIFFUSE       { "diffuse",     Colors::BLUE,     0.0, 0.00, 0.000 };
const Material DIAMOND       { "diamond",     Colors::WHITE,   25.0, 0.90, 2.417 };
const Material TRANSLUCENT   { "translucent", Colors::WHITE,  100.0, 0.90, 1.020 };
const Material MIRROR        { "mirror",      Colors::WHITE,  100.0, 1.00, 0.000 };

const Material& material(std::string name);

}

#endif
