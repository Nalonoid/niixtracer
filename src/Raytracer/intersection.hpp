#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include "Math/math.hpp"
#include "Object/object.hpp"

class Color;

class Intersection : public Object
{

public:
    Intersection();

    // Getters
    const Color& ks() const;
    const Color& kd() const;

private:
    Color _ks;
    Color _kd;

};

#endif
