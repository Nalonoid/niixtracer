#ifndef __PLANE_HPP__
#define __PLANE_HPP__

#include "Math/math.hpp"
#include "Image/color.hpp"
#include "shape.hpp"

class Plane : public Shape
{
public:
    Plane(const Vec3d &normal, double distance, const Color &color,
          const Material *mat, double emission = 0.0);

    Plane(const Vec3d &normal, double distance, const Color &color,
          const MaterialPBR *mat, double emission = 0.0);

    Plane(const Vec3d &normal, double distance, const Color &color,
          const MaterialPBR *mat, const Spectrum<> *emission_spctr);

    // Getters
    const Vec3d normal(const Vec3d& point = Space::ORIGIN) const override;
    Vec3d& normal();

    double distance() const;
    double& distance();

    // Methods
    bool intersect(Ray &r) override;

private:
    Vec3d   _normal;
    double  _distance;      // Distance from the plane to Space::ORIGIN
};

#endif
