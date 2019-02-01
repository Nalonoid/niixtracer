#include "triangle.hpp"
#include "Raytracer/ray.hpp"

Triangle::Triangle(const Vec3d &vA, const Vec3d &vB, const Vec3d &vC,
                   const Color &color, const Material *mat, double emission) :
    Shape(vA, mat, color, emission), _edge1(vB - vA), _edge2(vC - vA)
{
    std::cout << _position << std::endl;
    std::cout << vB << std::endl;
    std::cout << vC << std::endl;
    std::cout << _edge1 << std::endl;
    std::cout << _edge2 << std::endl;
}

Triangle::Triangle(const Vec3d &vA, const Vec3d &vB, const Vec3d &vC,
        const Color &color, const MaterialPBR *mat, double emission) :
    Shape(vA, mat, color, emission), _edge1(vB - vA), _edge2(vC - vA) {}


Triangle::Triangle(const Vec3d &vA, const Vec3d &vB, const Vec3d &vC,
        const MaterialPBR *mat, double emission) :
    Shape(vA, mat, emission), _edge1(vB - vA), _edge2(vC - vA) {}

Triangle::Triangle(const Vec3d &vA, const Vec3d &vB, const Vec3d &vC,
        const MaterialPBR *mat, const Spectrum<> *emission_spctr) :
    Shape(vA, mat, emission_spctr), _edge1(vB - vA), _edge2(vC - vA) {}

// Methods
bool Triangle::intersect(Ray &r)
{
    Vec3d dir       { r.direction()     };
    Vec3d h         { dir.cross(_edge2) };
    double det      { _edge1.dot(h)     };
    bool intersects { false             };

    // if det ~= 0 ray and triangle are parallel
    if (det < EPSILON && det > -EPSILON)
        return intersects;

    double inv_det = 1.0 / det;
    Vec3d s { r.origin() - _position };
    double u { inv_det * s.dot(h) };

    if (u < 0.0 || u > 1.0)
        return intersects;

    Vec3d q { s.cross(_edge1) };
    double v { dir.dot(q) * inv_det };

    if (v < 0.0 || u + v > 1.0)
        return intersects;

    double t { inv_det * _edge2.dot(q) };
    intersects = Shape::intersect(r, t);

    return intersects;
}

const Vec3d Triangle::normal(const Vec3d &) const
{
    return _edge1.cross(_edge2).normalized();
}
