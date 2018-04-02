#include "ray.hpp"

Ray::Ray(const Vec3d& ori, const Vec3d& dir) :
    _origin(ori), _direction(dir), _dist_max(20000) {}

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
