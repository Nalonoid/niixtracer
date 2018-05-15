#include "sphere.hpp"
#include "Raytracer/ray.hpp"

Sphere::Sphere() : Shape(), _radius(1.0) {}

Sphere::Sphere(const Vec3d &position, double radius) :
    Shape(position), _radius(radius) {}

Sphere::Sphere(const Vec3d &position, double radius, const Color &color) :
    Shape(position, Material(color)), _radius(radius) {}

Sphere::Sphere(const Vec3d &position, double radius, const Color &color,
       const Material &mat, double emission) :
    Shape(position, mat, emission), _radius(radius)
{
    material().color() = color;
}

// Getters
double Sphere::radius() const
{
    return _radius;
}

double& Sphere::radius()
{
    return _radius;
}

// Methods
bool Sphere::intersect(Ray &r)
{
    Vec3d  oc    { r.origin() - _position           };
    double b     { 2 * (oc.dot(r.direction()))      };
    double c     { oc.dot(oc) - _radius*_radius     };
    double discr { b*b - 4*c                        };

    bool   intersects  { false };
    double dist        { -1    };

    if (discr >= 0)
    {   
        discr = sqrt(discr);

        double t1 { -b - discr };
        double t2 { -b + discr };

        dist = t1 < t2 ? t1/2 : t2/2;

        intersects = Shape::intersect(r, dist);
    }

    return intersects;
}

const Vec3d Sphere::normal(const Vec3d &p) const
{
    return (p - _position).normalized();
}
