#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__

#include "Math/math.hpp"
#include "../object.hpp"
#include "../Material/material.hpp"
#include "../Light/light.hpp"

class Ray;

class Shape : public Object
{
public:   
    Shape(const Vec3d &position     = Space::ORIGIN,
          const Material &material  = Materials::DEFAULT,
          double emission = 0.0);

    Shape(const Material &material, double emission = 0.0);

    // Getters
    unsigned index() const override;

    const Material& material() const;
    Material& material();

    double emission() const;

    bool emits() const;

    // Methods
    virtual bool intersect(Ray &r) = 0;
    virtual const Vec3d normal(const Vec3d& point) const = 0;

    bool intersect(Ray &r, double dist);

private:
    static unsigned _index;
    Material _material;
    Light *_light;
};

#endif
