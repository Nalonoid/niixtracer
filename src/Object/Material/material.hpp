#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "Image/color.hpp"

struct Material
{
    // Getters
    const Color& color()      const;
    double       shininess()  const;
    double       reflection() const;
    double       refraction() const;

    Color& color();

    // Members
    Color   _color;
    double  _shininess;
    double  _reflection;
    double  _refraction;

    Material(Color c) :
        _color(c), _shininess(250), _reflection(0.2), _refraction(1.5) {}

    Material(Color c, double sh, double refl, double refr) :
        _color(c), _shininess(sh), _reflection(refl), _refraction(refr) {}
};

namespace Materials
{

const struct Material DEFAULT       { Colors::MAROON, 200.0, 0.01, 0.000 };
const struct Material PLASTIC       { Colors::CYAN,   200.0, 0.10, 0.000 };
const struct Material METAL         { Colors::CYAN,    10.0, 0.60, 0.000 };
const struct Material GLASS         { Colors::CYAN,    50.0, 0.50, 1.500 };
const struct Material DIFFUSE       { Colors::BLUE,     0.0, 0.00, 0.000 };
const struct Material GLOSSY        { Colors::WHITE,   25.0, 0.90, 2.417 };
const struct Material TRANSLUCENT   { Colors::WHITE,  100.0, 0.90, 1.020 };

}

#endif
