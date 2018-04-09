#include "plane.hpp"
#include "Raytracer/ray.hpp"

Plane::Plane(const Vec3d &normal, double distance) :
    Shape(Materials::DEFAULT), _normal(normal), _distance(distance) {}

Plane::Plane(const Vec3d &normal, double distance, const Color &color) :
    Shape(Material(color)), _normal(normal), _distance(distance) {}

Plane::Plane(const Vec3d &normal, double distance, const Material &mat) :
    Shape(mat), _normal(normal), _distance(distance) {}

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

bool Plane::intersect(Ray &r)
{
    double rd_n = r.direction().dot(_normal);
    double dist = -1;

    /* The ray is parallel to the plane if rd_n == 0. */
    if (rd_n != 0)
    {
        dist = (1/rd_n) * _normal.dot(_distance*_normal - r.origin());

        /* We return the distance from the origin of
        * the ray to the intersection point on the plane in t. */
        if (Shape::intersect(r, dist))
        {
            r.intersection().normal() = _normal;
            return true;
        }
    }

    return false;
}


const Vec3d Plane::normal_at(const Vec3d&)
{
    return Space::YAXIS;
}
