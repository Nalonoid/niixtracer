#ifndef __MATH_HPP__
#define __MATH_HPP__

template <typename T>
struct Vector
{
    // Constructors & structure members
    Vector() :
        x(0), y(0), z(0) {}

    Vector(T vx, T vy, T vz) :
        x(vx), y(vy), z(vz) {}

    T x;
    T y;
    T z;

    // Methods
    double magnitude() const;
    double dot(const Vector &v) const;

    Vector<T> normalized() const;
    Vector<T> negative() const;
    Vector<T> cross(const Vector<T> &v) const;
    Vector<T> add(const Vector<T> &v) const;
    Vector<T> mult(const double scalar) const;
};

// Operators
template <typename T>
Vector<T> operator+(const Vector<T> &v1, const Vector<T> &v2);

template <typename T>
Vector<T> operator-(const Vector<T> &v1, const Vector<T> &v2);

template <typename T>
Vector<T> operator*(const Vector<T> &v, const double scalar);

template <typename T>
Vector<T> operator*(const double scalar, const Vector<T> &v);

namespace Space
{

template <typename T = double>
struct Space
{
    Vector<T> xaxis;
    Vector<T> yaxis;
    Vector<T> zaxis;

    Space() :
        xaxis(Vector<double>(1, 0, 0)),
        yaxis(Vector<double>(0, 1, 0)),
        zaxis(Vector<double>(0, 0, 1)) {}

    Space(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z) :
        xaxis(x), yaxis(y), zaxis(z) {}
};

}

#include "math.inl"

#endif
