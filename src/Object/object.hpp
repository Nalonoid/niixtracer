#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "Math/math.hpp"
#include "Image/color.inl"

class Ray;

class Object
{
public:
    Object(const Vec3d &position = Space::ORIGIN,
           const Color &color = Colors::YELLOW);

    virtual ~Object();

    // Getters
    virtual unsigned index() const = 0;
    const Vec3d& position() const;
    Vec3d& position();
    const Color& color() const;
    Color& color();

protected:
    Vec3d _position;
    Color _color;
};

#endif
