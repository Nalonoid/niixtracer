#include <limits>

#include "ray.hpp"
#include "intersection.hpp"
#include "Utils/sampler.hpp"

unsigned long long int Ray::_number = 0;
unsigned long long int Ray::_over_number = 0;

Ray::Ray(const Vec3d& ori, const Vec3d& dir, const unsigned bnc,
         unsigned wavelength) : _origin(ori), _dist_max(10000), _bounces(bnc),
    _wavelength(wavelength)
{
    if (dir.magnitude() <= 1.0 - EPSILON || dir.magnitude() >= 1.0 + EPSILON)
        _direction = dir.normalized();
    else
        _direction = dir;

    if (_number < std::numeric_limits<unsigned long long>::max())
        _number++;
    else
        _over_number++;
}

// Getters
unsigned long long int Ray::number()
{
    return _number;
}

unsigned long long int Ray::over_number()
{
    return _over_number;
}

const Vec3d& Ray::direction() const
{
    return _direction;
}

const Vec3d& Ray::origin() const
{
    return _origin;
}

double Ray::dist_max() const
{
    return _dist_max;
}

unsigned Ray::bounces() const
{
    return _bounces;
}

unsigned Ray::wavelength() const
{
    return _wavelength;
}

const Intersection& Ray::intersection() const
{
    return _intersection;
}

Vec3d& Ray::direction()
{
    return _direction;
}

Vec3d& Ray::origin()
{
    return _origin;
}

double& Ray::dist_max()
{
    return _dist_max;
}

unsigned& Ray::bounces()
{
    return _bounces;
}

Intersection& Ray::intersection()
{
    return _intersection;
}

unsigned& Ray::wavelength()
{
    return _wavelength;
}
