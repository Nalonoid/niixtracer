#include "sphere.hpp"
#include "Raytracer/ray.hpp"

Sphere::Sphere() : Shape(), _radius(1.0) {}

Sphere::Sphere(const Vec3d &position, double radius) :
    Shape(position), _radius(radius) {}

Sphere::Sphere(const Vec3d &position, double radius, const Color &color) :
    Shape(position, color), _radius(radius) {}

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
        t *= -1;

        std::cout << "BLBLBLBL " << t << std::endl;

        Shape::intersect(r, t);
        return true;
    }

    t = -1;
    return false;
}

const Vec3d Sphere::normal_at(const Vec3d &p)
{
    return (p - _position).normalized();
}
