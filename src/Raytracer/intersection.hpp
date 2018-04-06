#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include "Object/object.hpp"
#include "Image/color.hpp"
#include "Object/Material/material.hpp"

class Intersection : public Object
{

public:
    Intersection();

    // Getters
    unsigned index() const override;
    bool exists() const;
    bool& exists();

    const Material* material() const;
    void set_material(Material *mat);

    const Vec3d& normal() const;

    Vec3d& normal();

private:
    bool  _exists;

    const Material* _material;
    Vec3d _normal;
};

#endif
