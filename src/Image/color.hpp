#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include "Math/math.hpp"

struct Color
{
    Color(int r, int g, int b, double alpha = 1) :
        rgb(Vec3i(r, g, b)), a(alpha) {}

    Vec3i rgb;
    double a;

    // Getters
    int r() const { return rgb.x; }
    int g() const { return rgb.y; }
    int b() const { return rgb.z; }

    int& r() { return rgb.x; }
    int& g() { return rgb.y; }
    int& b() { return rgb.z; }

    // Methods
    double  brightness() const;
    const Color& scale(float p);
};

namespace Colors
{
const struct Color BLACK    = {   0,   0,   0, 1. };
const struct Color WHITE    = { 255, 255, 255, 1. };
const struct Color GRAY     = { 128, 128, 128, 1. };
const struct Color RED      = { 255,   0,   0, 1. };
const struct Color GREEN    = {   0, 255,   0, 1. };
const struct Color BLUE     = {   0,   0, 255, 1. };
const struct Color MAGENTA  = { 255,   0, 255, 1. };
const struct Color YELLOW   = { 255, 255,   0, 1. };
const struct Color CYAN     = {   0, 255, 255, 1. };
const struct Color ORANGE   = { 255, 165,   0, 1. };
const struct Color PURPLE   = { 128,   0, 225, 1. };
const struct Color LIME     = { 160, 255,  60, 1. };
const struct Color MAROON   = { 128,  64,  64, 1. };

Color average(const Color &c1, const Color &c2);

}

#endif
