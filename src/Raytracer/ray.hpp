#ifndef __RAY_HPP__
#define __RAY_HPP__

#include "Math/math.hpp"

using Vec3d = Vector<double>;

class Ray
{

public:
    Ray();
    Ray(const Vec3d& dir, const Vec3d& ori);

    const Vec3d& direction() const;
    const Vec3d& origin() const;

    /* This second set of getters is useful to allow this kind of statement :
     * r.origin() = Vector(12, -3, 5); */
    Vec3d& direction();
    Vec3d& origin();

private:
    Vec3d _direction;
    Vec3d _origin;
};

#endif
