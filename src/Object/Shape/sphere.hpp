#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "Math/math.hpp"
#include "Image/color.hpp"
#include "shape.hpp"

using Vec3d = Vector<double>;

class Sphere : public Shape
{
public:
    Sphere(const Vec3d &position, double radius, const Color &color,
           const Material *mat, double emission = 0.0);

    Sphere(const Vec3d &position, double radius, const Color &color,
           const MaterialPBR *mat, double emission = 0.0);

    Sphere(const Vec3d &position, double radius, const Color &color,
           const MaterialPBR *mat, const Spectrum<> *emission_spctr);

    // Getters
    double radius() const;
    double& radius();

    // Methods
    bool intersect(Ray &r) override;
    const Vec3d normal(const Vec3d& point) const override;

private:
    double _radius;
};

#endif
