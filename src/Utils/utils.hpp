#ifndef __UTILS_HPP__
#define __UTILS_HPP__

inline double clamp(double value, double min, double max)
{
    return value < min ?
                min : value > max ?
                    max : value;
}

inline double max(double a, double b)
{
    return a > b ? a : b;
}

inline double min(double a, double b)
{
    return a < b ? a : b;
}

#endif
