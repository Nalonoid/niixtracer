#include "ray.hpp"

Ray::Ray() :
    _direction(Vec3d(0, 0, -1)), _origin(Vec3d(0, 0, 1)) {}

Ray::Ray(const Vec3d& dir, const Vec3d& ori) :
    _direction(dir), _origin(ori) {}

const Vec3d& Ray::direction() const
{
    return _direction;
}

const Vec3d& Ray::origin() const
{
    return _origin;
}

Vec3d& Ray::direction()
{
    return _direction;
}

Vec3d& Ray::origin()
{
    return _origin;
}
