#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__

#include "Math/math.hpp"
#include "../object.hpp"
#include "Material/material.hpp"
#include "Material/material_pbr.hpp"
#include "../Light/light.hpp"

class Ray;

class Shape : public Object
{
public:   
    Shape(const Vec3d &position     = Space::ORIGIN,
          const Material *material  = &Materials::DEFAULT,
          const Color &color        = Colors::WHITE,
          double emission           = 0.0);

    Shape(const Vec3d &position         = Space::ORIGIN,
          const MaterialPBR *material   = MaterialsPBR::material("matte"),
          double emission               = 0.0);

    Shape(const Vec3d &position         = Space::ORIGIN,
          const MaterialPBR *material   = MaterialsPBR::material("matte"),
          const Color &color            = Colors::WHITE,
          double emission               = 0.0);

    Shape(const Vec3d &position             = Space::ORIGIN,
          const MaterialPBR *material       = MaterialsPBR::material("matte"),
          const Spectrum<> *emission_spctr  = nullptr);

    Shape(const Material *material, const Color &color = Colors::WHITE,
          double emission = 0.0);

    Shape(const MaterialPBR *material, const Color &color = Colors::WHITE,
          double emission = 0.0);

    Shape(const MaterialPBR *material, double emission = 0.0);

    Shape(const MaterialPBR *material, const Spectrum<> *emission_spctr = nullptr);

    // Getters
    unsigned index() const override;

    const Material*     material() const;
    const MaterialPBR*  materialPBR() const;

    const Color& color() const;
    Color& color();

    double emission(unsigned wavelength = AVG_WAVELENGTH) const;

    bool emits() const;
    bool fluorescent() const;

    // Methods
    virtual bool intersect(Ray &r) = 0;
    virtual const Vec3d normal(const Vec3d& point) const = 0;

    bool intersect(Ray &r, double dist);

private:
    static unsigned     _index;
    const Material      *_material;
    const MaterialPBR   *_material_pbr;
    Color               _color;
    Light               *_light;
};

#endif
