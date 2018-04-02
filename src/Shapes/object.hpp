#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "Math/math.hpp"
#include "Raytracer/ray.hpp"
#include "Image/color.inl"

class Object
{
public:
    Object(const Color &color = Colors::YELLOW);

    // Getters
    const Color& color() const;
    Color& color();

    // Methods
    virtual bool intersect(Ray& r, double &dist) = 0;
    virtual const Vec3d normal_at(const Vec3d& point) = 0;

private:
    Color _color;
};

#endif
