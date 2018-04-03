#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__

#include "Math/math.hpp"
#include "../object.hpp"

class Ray;
class Color;

class Shape : public Object
{
public:   
    Shape(const Vec3d &position = Space::ORIGIN,
          const Color &color = Colors::YELLOW);

    Shape(const Color &color);

    // Getters
    unsigned index() const override;
    const Color& color() const;
    Color& color();

    // Methods
    virtual bool intersect(Ray &r, double &dist) = 0;
    virtual const Vec3d normal_at(const Vec3d& point) = 0;

private:
    static unsigned _index;
    Color _color;
};

#endif
