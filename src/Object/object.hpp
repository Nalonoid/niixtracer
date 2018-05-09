#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "Math/math.hpp"

class Object
{
public:
    Object(const Vec3d &position = Space::ORIGIN);

    virtual ~Object();

    // Getters
    virtual unsigned index() const = 0;
    const Vec3d& position() const;

    Vec3d& position();

protected:
    Vec3d _position;
};

#endif
