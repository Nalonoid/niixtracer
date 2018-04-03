#include "intersection.hpp"

Intersection::Intersection() : _ks(Colors::BLACK), _kd(Colors::BLACK) {}

const Color& Intersection::ks() const
{
    return _ks;
}

const Color& Intersection::kd() const
{
    return _kd;
}
