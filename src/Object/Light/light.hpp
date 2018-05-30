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
#include "Raytracer/spectrum.hpp"

class Light : public Object
{
public:
    Light(double emission, const Vec3d &p = Space::ORIGIN,
          const Color &c = Colors::WHITE);

    Light(const Spectrum<> *s = Spectra::BLACK_BODY_D65,
          const Vec3d &p = Space::ORIGIN,
          const Color& c = Colors::WHITE);

    // Getters
    unsigned index() const override;
    const Color& color() const;
    double emission(unsigned wavelength = AVG_WAVELENGTH) const;
    double brightness() const;

private:
    static unsigned _index;
    Color _color;
    const Spectrum<> *_emission_spectrum;
};

#endif
