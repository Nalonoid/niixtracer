#ifndef __PLANE_HPP__
#define __PLANE_HPP__

#include "Math/math.hpp"
#include "Image/color.hpp"
#include "shape.hpp"

class Plane : public Shape
{
public:
    Plane(const Vec3d &normal = Space::YAXIS, double distance = 0.0);
    Plane(const Vec3d &normal, double distance, const Color &color);
    Plane(const Vec3d &normal, double distance, const Material &mat);

    // Getters
    const Vec3d& normal() const;
    Vec3d& normal();

    double distance() const;
    double& distance();

    // Methods
    bool intersect(Ray &r, double &dist) override;
    const Vec3d normal_at(const Vec3d& point) override;

private:
    Vec3d   _normal;
    double  _distance;      // Distance from the plane to Space::ORIGIN
};

#endif
