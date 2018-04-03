#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "Math/math.hpp"
#include "Image/color.inl"
#include "../object.hpp"

class Light : public Object
{
public:
    Light();
    Light(const Vec3d &p, const Color &c = Colors::WHITE);

    // Getters
    unsigned index() const override;

private:
    static unsigned _index;
    Color _color;
};

#endif
