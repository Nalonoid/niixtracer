#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include "Math/math.hpp"

struct Color
{
    Color(double r, double g, double b, double alpha = 1) :
        rgb(Vec3d(r, g, b)), a(alpha) {}

    Color(int r, int g, int b, double alpha = 1) :
        rgb(Vec3d(r/255.0, g/255.0, b/255.0)), a(alpha) {}

    Color(Vec3d rgb_v, double alpha = 1) :
        rgb(rgb_v), a(alpha) {}

    Vec3d rgb;
    double a;

    // Getters
    double r() const { return rgb.x; }
    double g() const { return rgb.y; }
    double b() const { return rgb.z; }

    double& r() { return rgb.x; }
    double& g() { return rgb.y; }
    double& b() { return rgb.z; }

    // Methods
    const Color& scale(float p);
    const Color& clamp();
    double       brightness() const;
};

// Operators
Color operator+(const Color &c1, const Color &c2);
Color operator-(const Color &c1, const Color &c2);
Color operator*(const Color &c, const double scalar);
Color operator*(const double scalar, const Color &c);
Color operator*(const Color &c1, const Color &c2);

namespace Colors
{
const struct Color BLACK    { 0.0 , 0.0 , 0.0 , 1. };
const struct Color WHITE    { 1.0 , 1.0 , 1.0 , 1. };
const struct Color GRAY     { 0.5 , 0.5 , 0.5 , 1. };
const struct Color RED      { 1.0 , 0.0 , 0.0 , 1. };
const struct Color GREEN    { 0.0 , 1.0 , 0.0 , 1. };
const struct Color BLUE     { 0.0 , 0.0 , 1.0 , 1. };
const struct Color MAGENTA  { 1.0 , 0.0 , 1.0 , 1. };
const struct Color YELLOW   { 1.0 , 1.0 , 0.0 , 1. };
const struct Color CYAN     { 0.0 , 1.0 , 1.0 , 1. };
const struct Color ORANGE   { 1.0 , 0.65, 0.0 , 1. };
const struct Color PURPLE   { 0.5 , 0.0,  1.0 , 1. };
const struct Color LIME     { 0.63, 1.0 , 0.23, 1. };
const struct Color MAROON   { 0.5 , 0.25, 0.25, 1. };

Color average(const Color &c1, const Color &c2);

}

#endif
