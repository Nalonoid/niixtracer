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

    // Members
    Color   _color;
    double  _shininess;
    double  _reflection;
    double  _refraction;

    Material(Color c) :
        _color(c), _shininess(0.25), _reflection(0.2), _refraction(0.0) {}

    Material(Color c, double sh, double refl, double refr) :
        _color(c), _shininess(sh), _reflection(refl), _refraction(refr) {}
};

namespace Materials
{

const struct Material DEFAULT = { Colors::MAROON, 1.0, 0.8, 0.0 };
const struct Material PLASTIC = { Colors::CYAN, 1.0, 0.8, 0.0 };

}

#endif
