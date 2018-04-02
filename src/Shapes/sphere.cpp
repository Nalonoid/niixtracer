#include "sphere.hpp"

Sphere::Sphere() : Object(), _position(Space::ORIGIN), _radius(1.0) {}

Sphere::Sphere(const Vec3d &position, double radius) :
    _position(position), _radius(radius) {}

Sphere::Sphere(const Vec3d &position, double radius, const Color &color) :
    Object(color), _position(position), _radius(radius) {}

// Getters
const Vec3d &Sphere::position() const
{
    return _position;
}

Vec3d &Sphere::position()
{
    return _position;
}

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
    double a     = r.direction().dot(r.direction());
    double b     = 2*(oc.dot(r.direction()));
    double c     = (oc.dot(oc)) - (_radius*_radius);
    double discr = b*b - 4*c;

    if (discr > 0)
    {   
        float t1 = ((-1)*b - sqrt(discr))/2*a;
        float t2 = ((-1)*b + sqrt(discr))/2*a;
        t = t1 < t2 ? t1 : t2;
        t -= 0.0000001;

        return true;
    }

    t = -1;
    return false;
}

const Vec3d Sphere::normal_at(const Vec3d &p)
{
    return (p - _position).normalized();
}
