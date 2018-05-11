#ifndef __MATH_HPP__
#define __MATH_HPP__

#include <ostream>

namespace {
    float const EPSILON { 0.001 };
    float const PI      { 3.141592653589 };
}

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
    Vector<T> add(const Vector<T> &v) const;
    Vector<T> mult(const double scalar) const;
    Vector<T> reflect(const Vector<T> &normal) const;
    Vector<T> cross(const Vector<T> &v) const;

    std::string to_string() const;
};

// Using...
using Vec3d = Vector<double>;
using Vec3f = Vector<float>;
using Vec3i = Vector<int>;

// Methods
template <typename T>
Vector<T> cross(const Vector<T> &v1, const Vector<T> &v2);

// Operators
template <typename T>
Vector<T> operator+(const Vector<T> &v1, const Vector<T> &v2);

template <typename T>
Vector<T> operator-(const Vector<T> &v1, const Vector<T> &v2);

template <typename T>
Vector<T> operator*(const Vector<T> &v, const double scalar);

template <typename T>
Vector<T> operator*(const double scalar, const Vector<T> &v);

template <typename T>
Vector<T> operator/(const Vector<T> &v,const double scalar);

template <typename T>
std::ostream& operator<<(std::ostream &os, const Vector<T> &v);

// Space
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

// Create an orthonormal basis from the vector v1
template <typename T>
void orthonormal_basis(const Vector<T>& v1, Vector<T>& v2, Vector<T>& v3);

}

#include "math.inl"

#endif
