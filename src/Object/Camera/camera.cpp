#include "camera.hpp"

unsigned Camera::_index = 0;

Camera::Camera() : Object(Vec3d(0, 2, -5))
{
    _index++;
    look_at(Vec3d(0, 0, 0));
    compute_camera_basis();
}

Camera::Camera(const Vec3d& pos, const Vec3d& la) :
    Object(pos)
{
    _index++;
    look_at(la);
    compute_camera_basis();
}

void Camera::compute_camera_basis()
{
    _up     = cross(_direction, Space::XAXIS).normalized();
    _left  = cross(_up, _direction).normalized();
}

// Getters
unsigned Camera::index() const
{
    return _index;
}

const Vec3d& Camera::direction() const
{
    return _direction;
}

Vec3d& Camera::direction()
{
    return _direction;
}

const Vec3d& Camera::up() const
{
    return _up;
}

Vec3d& Camera::up()
{
    return _up;
}

const Vec3d& Camera::left() const
{
    return _left;
}

Vec3d& Camera::left()
{
    return _left;
}

const Vec3d& Camera::look_at() const
{
    return _look_at;
}

Vec3d& Camera::look_at(Vec3d point)
{
    _look_at = point;
    _direction = (_look_at - _position).normalized();
    return _look_at;
}
