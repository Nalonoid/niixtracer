#ifndef __UTILS_HPP__
#define __UTILS_HPP__

inline float clamp(float value, float min, float max)
{
    return value < min ?
                min : value > max ?
                    max : value;
}

inline float max(float a, float b)
{
    return a > b ? a : b;
}

inline float min(float a, float b)
{
    return a < b ? a : b;
}

#endif
