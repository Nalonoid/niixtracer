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
    Light(const Vec3d &p = Space::ORIGIN,
          const Color &c = Colors::WHITE, double emission = 500.0);

    // Getters
    unsigned index() const override;
    const Color& color() const;
    double emission() const;
    double brightness() const;

    double& emission();

private:
    static unsigned _index;
    Color _color;
    double _emission;
};

#endif
