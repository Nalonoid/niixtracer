#include "intersection.hpp"

Intersection::Intersection() : _exists(false), _ks(Colors::BLACK), _kd(Colors::BLACK) {}

unsigned Intersection::index() const
{
    return 0;
}

bool Intersection::exists() const
{
    return _exists;
}

bool& Intersection::exists()
{
    return _exists;
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

const Vec3d& Intersection::normal() const
{
    return _normal;
}

Vec3d& Intersection::normal()
{
    return _normal;
}
