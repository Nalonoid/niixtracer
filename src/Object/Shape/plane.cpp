#include "plane.hpp"
#include "Raytracer/ray.hpp"

Plane::Plane(const Vec3d &normal, double distance) :
    _normal(normal), _distance(distance) {}

Plane::Plane(const Vec3d &normal, double distance, const Color &color) :
    Shape(color), _normal(normal), _distance(distance) {}

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

bool Plane::intersect(Ray &r, double &t)
{
    double rd_n = r.direction().dot(_normal);
    t = -1;

    /* The ray is parallel to the plane if rd_n == 0. */
    if (rd_n != 0)
    {
        t = (1/rd_n) * _normal.dot(_distance*_normal - r.origin());

        /* We return the distance from the origin of
        * the ray to the intersection point on the plane in t. */
        Shape::intersect(r, t);
        return true;
    }

    return false;
}


const Vec3d Plane::normal_at(const Vec3d&)
{
    return Space::YAXIS;
}
