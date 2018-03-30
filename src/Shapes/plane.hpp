#ifndef __PLANE_HPP__
#define __PLANE_HPP__

#include "Math/math.hpp"
#include "Image/color.inl"
#include "object.hpp"

using Vec3d = Vector<double>;

class Plane : Object
{
public:
    Plane(const Vec3d &normal = Space::YAXIS, double distance = 0.0);
    Plane(const Vec3d &normal, double distance, const Color &color);

    // Getters
    const Vec3d& normal() const;
    Vec3d& normal();

    double distance() const;
    double& distance();

    // Methods
    double intersection(Ray r) override;
    const Vec3d& normal_at(Vec3d point) override;

private:
    Vec3d   _normal;
    double  _distance;      // Distance from the plane to Space::ORIGIN
};

#endif
