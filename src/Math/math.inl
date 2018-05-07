#include <math.h>
#include <iostream>

// Vectors

template <typename T>
double Vector<T>::magnitude() const
{
    // i.e. the square root of the dot product
    return sqrt(x*x + y*y + z*z);
}

template <typename T>
double Vector<T>::dot(const Vector<T> &v) const
{
    return x*v.x + y*v.y + z*v.z;
}

template <typename T>
Vector<T> Vector<T>::normalized() const
{
    double m { magnitude() };

    return Vector<T>(x/m, y/m, z/m);
}

template <typename T>
Vector<T> Vector<T>::negative() const
{
    return Vector<T>(-x, -y, -z);
}

template <typename T>
Vector<T> Vector<T>::cross(const Vector<T> &v) const
{
    return Vector<T>(y*v.z - z*v.y,
                     z*v.x - x*v.z,
                     x*v.y - y*v.x);
}

template <typename T>
Vector<T> Vector<T>::add(const Vector<T> &v) const
{
    return Vector<T>(x + v.x, y + v.y, z + v.z);
}

template <typename T>
Vector<T> Vector<T>::mult(const double scalar) const
{
    return Vector<T>(x * scalar, y * scalar, z * scalar);
}

template <typename T>
Vector<T> Vector<T>::reflect(const Vector<T> &normal) const
{
    return (*this) - 2 * ((*this).dot(normal) * normal);
}

// Function overloading operators
template <typename T>
Vector<T> operator+(const Vector<T> &v1, const Vector<T> &v2)
{
    return v1.add(v2);
}

template <typename T>
Vector<T> operator-(const Vector<T> &v1, const Vector<T> &v2)
{
    return v1.add(v2.negative());
}

template <typename T>
Vector<T> operator*(const Vector<T> &v, const double scalar)
{
    return v.mult(scalar);
}

template <typename T>
Vector<T> operator*(const double scalar, const Vector<T> &v)
{
    return v.mult(scalar);
}

template <typename T>
Vector<T> operator/(const Vector<T> &v,const double scalar)
{
    return Vector<T>(v.x / scalar, v.y / scalar, v.z / scalar);
}

template <typename T>
std::ostream& operator<<(std::ostream &os, const Vector<T> &v)
{
    return os << "(" << v.x << "," << v.y << "," << v.z << ")";
}

template <typename T>
std::string Vector<T>::to_string() const
{
    return std::to_string(x) + ", " +
           std::to_string(y) + ", " +
           std::to_string(z);
}

// Space
namespace Space
{

const struct Space<> ORTHONORMAL_BASIS;
const Vector<double> ORIGIN { 0, 0, 0 };
const Vector<double> XAXIS  { 1, 0, 0 };
const Vector<double> YAXIS  { 0, 1, 0 };
const Vector<double> ZAXIS  { 0, 0, 1 };

template <typename T>
void orthonormal_basis(const Vector<T>& v1, Vector<T>& v2, Vector<T>& v3)
{
    v2 = Vec3d(0, -v1.z, v1.y);
    v3 = v1.cross(v2);

    v2 = v2.normalized();
    v3 = v3.normalized();
}

}
