#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

/* This class is obsolete if you're using the program in the pathtracing mode
 * It may be used to create point light sources in ratracing mode.
 *
 * Using the pathtracer algorithm you would have to create a shape with non-zero
 * emission.
 */

#include "Math/math.hpp"
#include "Image/color.hpp"
#include "../object.hpp"

class Light : public Object
{
public:
    Light();
    Light(const Vec3d &p, const Color &c = Colors::WHITE);

    // Getters
    unsigned index() const override;
    const Color& color() const;
    double brightness() const;

private:
    static unsigned _index;
    Color _color;
};

#endif
