#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "Math/math.hpp"
#include "Image/color.hpp"
#include "shape.hpp"

using Vec3d = Vector<double>;

class Sphere : public Shape
{
public:
    Sphere();
    Sphere(const Vec3d &position, double radius = 1.0);
    Sphere(const Vec3d &position, double radius, const Color &color);
    Sphere(const Vec3d &position, double radius, const Color &color,
           const Material &mat);

    // Getters
    double radius() const;
    double& radius();

    // Methods
    bool intersect(Ray &r) override;
    const Vec3d normal_at(const Vec3d& point) override;

private:
    double _radius;
};

#endif
