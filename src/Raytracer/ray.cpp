#include "ray.hpp"
#include "intersection.hpp"

unsigned Ray::_number = 0;

Ray::Ray(const Vec3d& ori, const Vec3d& dir) :
    _origin(ori), _direction(dir), _dist_max(20000), _bounces(0)
{
    _number++;
}

// Methods
void Ray::init(const Vec3d& ori, const Vec3d& dir)
{
    _origin = ori;
    _direction = dir;
}

// Getters
unsigned Ray::number()
{
    return _number;
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

Intersection& Ray::intersection()
{
    return _intersection;
}
