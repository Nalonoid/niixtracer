#include "camera.hpp"

Camera::Camera()
{
    _position = Vec3d(960, 540, 0);
    look_at(Vec3d(0, 0, 0));
    compute_camera_basis();
}

Camera::Camera(const Vec3d& pos, const Vec3d& la) :
    _position(pos)
{
    look_at(la);
    compute_camera_basis();
}

void Camera::compute_camera_basis()
{
    _right      = cross(_direction, Space::YAXIS).normalized();
    _up         = cross(_right, _direction);
}

const Vec3d& Camera::direction() const
{
    return _direction;
}

Vec3d& Camera::direction()
{
    return _direction;
}

const Vec3d& Camera::position() const
{
    return _position;
}

Vec3d& Camera::position()
{
    return _position;
}

const Vec3d& Camera::up() const
{
    return _up;
}

Vec3d& Camera::up()
{
    return _up;
}

const Vec3d& Camera::right() const
{
    return _right;
}

Vec3d& Camera::right()
{
    return _right;
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
