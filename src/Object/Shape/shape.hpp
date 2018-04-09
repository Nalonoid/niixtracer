#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__

#include "Math/math.hpp"
#include "../object.hpp"
#include "../Material/material.hpp"

class Ray;

class Shape : public Object
{
public:   
    Shape(const Vec3d &position     = Space::ORIGIN,
          const Material &material  = Materials::DEFAULT);

    Shape(const Material &material);

    // Getters
    unsigned index() const override;
    const Material& material() const;
    Material& material();

    // Methods
    virtual bool intersect(Ray &r) = 0;
    virtual const Vec3d normal_at(const Vec3d& point) = 0;

    bool intersect(Ray &r, double dist);

private:
    static unsigned _index;
    Material _material;
};

#endif
