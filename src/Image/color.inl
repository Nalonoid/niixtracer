#ifndef __COLOR_INL__
#define __COLOR_INL__

#include "Math/math.hpp"

struct Color : Vector<int>
{
    Color(int r, int g, int b, double alpha = 1) :
        Vector<int>(r, g, b), a(alpha) {}

    double a;
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
}

#endif
