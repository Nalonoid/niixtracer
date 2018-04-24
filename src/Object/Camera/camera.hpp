#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Math/math.hpp"
#include "../object.hpp"

class Camera : public Object
{

public:
    Camera();
    Camera(const Vec3d& pos, const Vec3d& la);
    Camera(const Vec3d& pos, const Vec3d& la, double ar);

    // Getters
    unsigned index() const override;

    const Vec3d& direction() const;
    const Vec3d& up() const;
    const Vec3d& left() const;
    const Vec3d& look_at() const;

    /* This second set of getters is useful to allow this kind of statement :
     * c.direction() = Vector(12, -3, 5); */
    Vec3d& direction();
    Vec3d& up();
    Vec3d& left();

    // Methods
    Vec3d& look_at(Vec3d point);

private:
    // Private methods
    void compute_camera_basis();

private:
    static unsigned _index;
    double _aspect_ratio;

    Vec3d _direction;
    Vec3d _look_at;
    Vec3d _left;
    Vec3d _up;
};

#endif
