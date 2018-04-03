#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include "Object/object.hpp"

class Color;

class Intersection : public Object
{

public:
    Intersection();

    // Getters
    unsigned index() const override;
    const Color& ks() const;
    const Color& kd() const;

    Color& ks();
    Color& kd();

private:
    Color _ks;
    Color _kd;

};

#endif
