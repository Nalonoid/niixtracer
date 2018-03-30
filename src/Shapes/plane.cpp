#include "plane.hpp"

Plane::Plane(const Vec3d &normal, double distance) :
    _normal(normal), _distance(distance) {}

Plane::Plane(const Vec3d &normal, double distance, const Color &color) :
    Object(color), _normal(normal), _distance(distance) {}

const Vec3d& Plane::normal() const
{
    return _normal;
}

Vec3d& Plane::normal()
{
    return _normal;
}

double Plane::distance() const
{
    return _distance;
}

double& Plane::distance()
{
    return _distance;
}

double Plane::intersection(Ray r)
{
    double rd_n = r.direction().dot(_normal);

    if (rd_n == 0)  // The ray is parallel to the plane
        return -1.0;

    /* The ray intersects the plane, we return the distance from the origin of
     * the ray to the intersection point on the plane. */
    return (1/rd_n) * _normal.dot(_distance*_normal - r.origin());
}


const Vec3d& Plane::normal_at(Vec3d)
{
    return Space::YAXIS;
}
