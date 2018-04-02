#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Math/math.hpp"

using Vec3d = Vector<double>;

class Camera
{

public:
    Camera();
    Camera(const Vec3d& pos, const Vec3d& la);

    // Getters
    const Vec3d& direction() const;
    const Vec3d& position() const;
    const Vec3d& up() const;
    const Vec3d& right() const;
    const Vec3d& look_at() const;

    /* This second set of getters is useful to allow this kind of statement :
     * c.direction() = Vector(12, -3, 5); */
    Vec3d& direction();
    Vec3d& position();
    Vec3d& up();
    Vec3d& right();

    // Methods
    Vec3d& look_at(Vec3d point);

private:
    // Private methods
    void compute_camera_basis();

private:
    Vec3d _direction;
    Vec3d _position;
    Vec3d _look_at;
    Vec3d _right;
    Vec3d _up;
};

#endif
