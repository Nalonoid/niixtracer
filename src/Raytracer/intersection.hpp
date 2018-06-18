#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include "Object/object.hpp"
#include "Object/Shape/shape.hpp"
#include "Image/color.hpp"
#include "Material/material.hpp"

class Intersection : public Object
{
public:
    Intersection();

    // Getters
    unsigned index() const override;
    const Material* material() const;
    const Shape* shape() const;
    void set_shape(Shape *shape);
    const Vec3d& normal() const;

    Vec3d& normal();

private:
    Vec3d _normal;
    const Shape *_shape;
};

#endif
