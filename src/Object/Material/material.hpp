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
        _color(c), _shininess(250), _reflection(0.2), _refraction(0.0) {}

    Material(Color c, double sh, double refl, double refr) :
        _color(c), _shininess(sh), _reflection(refl), _refraction(refr) {}
};

namespace Materials
{

const struct Material DEFAULT { Colors::MAROON,   1.0, 0.2, 1.3 };
const struct Material PLASTIC { Colors::CYAN,   100.0, 0.2, 1.6 };
const struct Material GLASS   { Colors::CYAN,    50.0, 0.2, 1.5 };
const struct Material DIFFUSE { Colors::BLUE,     0.0, 0.0, 0.0 };


}

#endif
