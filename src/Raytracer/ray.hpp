#ifndef __RAY_HPP__
#define __RAY_HPP__

#include "Math/math.hpp"
#include "intersection.hpp"

class Ray
{

public:
    Ray(const Vec3d& ori = Vec3d(0, 0, 0),
        const Vec3d& dir = Space::ZAXIS,
        const unsigned bnc = 0,
        unsigned wavelength = MIN_WAVELENGTH);

    // Getters
    static unsigned long long int number();
    static unsigned long long int over_number();

    const Vec3d& origin() const;
    const Vec3d& direction() const;
    double dist_max() const;
    unsigned bounces() const;
    const Intersection& intersection() const;
    unsigned wavelength() const;

    /* This second set of getters is useful to allow this kind of statement :
     * r.origin() = Vector(12, -3, 5); */
    Vec3d& origin();
    Vec3d& direction();
    double& dist_max();
    unsigned& bounces();
    Intersection& intersection();
    unsigned& wavelength();

private:
    static unsigned long long int _number;
    static unsigned long long int _over_number;

    Vec3d           _origin;
    Vec3d           _direction;
    double          _dist_max;
    unsigned        _bounces;
    Intersection    _intersection;
    unsigned        _wavelength;
};

#endif
