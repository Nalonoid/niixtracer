#ifndef __RAY_HPP__
#define __RAY_HPP__

#include "Math/math.hpp"

class Ray
{

public:
    Ray(const Vec3d& ori = Vec3d(0, 0, 0),
        const Vec3d& dir = Space::ZAXIS);

    const Vec3d& origin() const;
    const Vec3d& direction() const;
    double dist_max() const;

    /* This second set of getters is useful to allow this kind of statement :
     * r.origin() = Vector(12, -3, 5); */
    Vec3d& origin();
    Vec3d& direction();
    double& dist_max();

private:
    Vec3d  _origin;
    Vec3d  _direction;
    double _dist_max;
};

#endif
