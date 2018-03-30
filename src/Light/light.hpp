#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "Math/math.hpp"
#include "Image/color.inl"

using Vec3d = Vector<double>;

class Light
{
public:
    Light();
    Light(const Vec3d &p, const Color &c = Colors::WHITE);

    // Getters
    const Vec3d& position() const;
    const Color& color() const;

private:
    Vec3d _position;
    Color _color;

};

#endif
