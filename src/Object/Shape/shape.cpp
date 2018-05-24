#include "shape.hpp"
#include "Raytracer/ray.hpp"
#include "Raytracer/intersection.hpp"

unsigned Shape::_index = 0;

Shape::Shape(const Vec3d &position, const Material *material,
             const Color &color, double emission) :
    Object(position), _material(material), _material_pbr(nullptr), _color(color)
{
    _index++;
    _light = emission > 0.0 ? new Light(position, color, emission)
                            : nullptr;
}

Shape::Shape(const Vec3d &position, const MaterialPBR *material,
             const Color &color, double emission) :
    Object(position), _material(nullptr), _material_pbr(material), _color(color)
{
    _index++;
    _light = emission > 0.0 ? new Light(position, color, emission)
                            : nullptr;
}

Shape::Shape(const Material *material, const Color &color, double emission) :
    Object(Space::ORIGIN), _material(material), _material_pbr(nullptr), _color(color)
{
    _index++;
    _light = emission > 0.0 ? new Light(Space::ORIGIN, emission) : nullptr;
}

Shape::Shape(const MaterialPBR *material, const Color &color, double emission) :
    Object(Space::ORIGIN), _material(nullptr), _material_pbr(material), _color(color)
{
    _index++;
    _light = emission > 0.0 ? new Light(Space::ORIGIN, emission) : nullptr;
}


// Getters
unsigned Shape::index() const
{
    return _index;
}

const Material* Shape::material() const
{
    return _material;
}

const MaterialPBR* Shape::materialPBR() const
{
    return _material_pbr;
}

const Color& Shape::color() const
{
    return _color;
}

Color &Shape::color()
{
    return _color;
}

double Shape::emission() const
{
    return (_light != nullptr) ? _light->emission() : 0.0;
}

bool Shape::emits() const
{
    return _light != nullptr;
}

// Methods
bool Shape::intersect(Ray &r, double t)
{   
    if (t > 0 && t < r.dist_max())
    {
        r.dist_max() = t;
        r.intersection().position() = r.origin() + (t * r.direction());
        r.intersection().normal() = normal(r.intersection().position());
        r.intersection().set_shape(this);
        return true;
    }

    return false;
}

const Vec3d Shape::normal(const Vec3d&) const
{
    return Space::YAXIS;
}
