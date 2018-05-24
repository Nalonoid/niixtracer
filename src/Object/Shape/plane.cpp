#include "plane.hpp"
#include "Raytracer/ray.hpp"

Plane::Plane(const Vec3d &normal, double distance, const Color &color,
      const Material *mat, double emission) :
    Shape(mat, color, emission), _normal(normal), _distance(distance) {}

Plane::Plane(const Vec3d &normal, double distance, const Color &color,
      const MaterialPBR *mat, double emission) :
    Shape(mat, color, emission), _normal(normal), _distance(distance) {}

const Vec3d Plane::normal(const Vec3d&) const
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
    double  dist        { -1    };
    bool    intersects  { false };

    double rd_n { r.direction().dot(_normal) };

    /* The ray is parallel to the plane if rd_n == 0. */
    if (rd_n != 0)
    {
        dist = (1/rd_n) * _normal.dot(_distance*_normal - r.origin());
        intersects = Shape::intersect(r, dist);
    }

    return intersects;
}
