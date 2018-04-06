#include "sphere.hpp"
#include "Raytracer/ray.hpp"

Sphere::Sphere() : Shape(), _radius(1.0) {}

Sphere::Sphere(const Vec3d &position, double radius) :
    Shape(position), _radius(radius) {}

Sphere::Sphere(const Vec3d &position, double radius, const Color &color) :
    Shape(position, Material(color)), _radius(radius) {}

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
bool Sphere::intersect(Ray &r, double &t)
{
    Vec3d  oc    = r.origin() - _position;
    double b     = 2*(oc.dot(r.direction()));
    double c     = (oc.dot(oc)) - (_radius*_radius);
    double discr = b*b - 4*c;

    t = -1;

    if (discr > 0)
    {   
        float t1 = ((-1)*b - sqrt(discr))*0.5;
        float t2 = ((-1)*b + sqrt(discr))*0.5;
        t = t1 < t2 ? t1 : t2;

        Shape::intersect(r, t);
        r.intersection().normal() =
                (r.intersection().position() - _position).normalized();
    }

    return r.intersection().exists();
}

const Vec3d Sphere::normal_at(const Vec3d &p)
{
    return (p - _position).normalized();
}
