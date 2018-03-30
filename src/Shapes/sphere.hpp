#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "Math/math.hpp"
#include "Image/color.inl"
#include "object.hpp"

using Vec3d = Vector<double>;

class Sphere : Object
{
public:
    Sphere();
    Sphere(const Vec3d &position, double radius = 1.0);
    Sphere(const Vec3d &position, double radius, const Color &color);

    // Getters
    const Vec3d& position() const;
    Vec3d& position();

    double radius() const;
    double& radius();

    // Methods
    double intersection(Ray r);
    const Vec3d& normal_at(Vec3d point);

private:
    Vec3d _position;
    double _radius;
};

#endif
