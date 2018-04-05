#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include "Object/object.hpp"
#include "Image/color.hpp"

class Intersection : public Object
{

public:
    Intersection();

    // Getters
    unsigned index() const override;
    bool exists() const;
    bool& exists();

    const Color& ks()     const;
    const Color& kd()     const;
    const Vec3d& normal() const;

    Color& ks();
    Color& kd();
    Vec3d& normal();

private:
    bool  _exists;

    Color _ks;
    Color _kd;
    Vec3d _normal;
};

#endif
