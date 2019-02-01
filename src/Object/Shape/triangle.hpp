#ifndef __TRIANGLE_HPP__
#define __TRIANGLE_HPP__

#include "Math/math.hpp"
#include "Image/color.hpp"
#include "shape.hpp"

using Vec3d = Vector<double>;

class Triangle : public Shape
{
public:
    Triangle(const Vec3d &vA, const Vec3d &vB, const Vec3d &vC,
             const Color &color, const Material *mat, double emission = 0.0);

    Triangle(const Vec3d &vA, const Vec3d &vB, const Vec3d &vC,
             const Color &color, const MaterialPBR *mat, double emission = 0.0);

    Triangle(const Vec3d &vA, const Vec3d &vB, const Vec3d &vC,
             const MaterialPBR *mat, double emisison = 0.0);

    Triangle(const Vec3d &vA, const Vec3d &vB, const Vec3d &vC,
             const MaterialPBR *mat, const Spectrum<> *emission_spctr);

    // Methods
    bool intersect(Ray &r) override;
    const Vec3d normal(const Vec3d& point) const override;

private:
    Vec3d _edge1;
    Vec3d _edge2;
};

#endif
