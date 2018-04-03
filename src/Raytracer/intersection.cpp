#include "intersection.hpp"

Intersection::Intersection() : _ks(Colors::BLACK), _kd(Colors::BLACK) {}

unsigned Intersection::index() const
{
    return 0;
}

const Color& Intersection::ks() const
{
    return _ks;
}

const Color& Intersection::kd() const
{
    return _kd;
}

Color& Intersection::ks()
{
    return _ks;
}

Color& Intersection::kd()
{
    return _kd;
}
