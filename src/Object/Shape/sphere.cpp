#include "sphere.hpp"
#include "Raytracer/ray.hpp"

Sphere::Sphere() : Shape(), _radius(1.0) {}

Sphere::Sphere(const Vec3d &position, double radius) :
    Shape(position), _radius(radius) {}

Sphere::Sphere(const Vec3d &position, double radius, const Color &color) :
    Shape(position, Material(color)), _radius(radius) {}

Sphere::Sphere(const Vec3d &position, double radius, const Color &color,
       const Material &mat) :
    Shape(position, mat), _radius(radius)
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
    double b     { 2*(oc.dot(r.direction()))        };
    double c     { (oc.dot(oc)) - (_radius*_radius) };
    double discr { b*b - 4*c                        };

    bool   intersects  { false };
    double dist        { -1    };

    if (discr > 0)
    {   
        double t1 { ((-1)*b - sqrt(discr))*0.5 };
        double t2 { ((-1)*b + sqrt(discr))*0.5 };

        dist = t1 < t2 ? t1 : t2;

        if ((intersects = Shape::intersect(r, dist)))
            r.intersection().normal() =
                    (r.intersection().position() - _position).normalized();
    }

    return intersects;
}

const Vec3d Sphere::normal_at(const Vec3d &p)
{
    return (p - _position).normalized();
}
