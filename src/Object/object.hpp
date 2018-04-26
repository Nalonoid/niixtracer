#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "Math/math.hpp"

class Object
{
public:
    enum class OBJECT_TYPE { SHAPE, LIGHT, CAMERA };

    Object(const Vec3d &position = Space::ORIGIN, double emission = 0.0);

    virtual ~Object();

    // Getters
    virtual unsigned index() const = 0;
    const Vec3d& position() const;
    double emission() const;

    Vec3d& position();
    double& emission();

protected:
    OBJECT_TYPE _type;
    Vec3d _position;
    double _emission;
};

#endif
