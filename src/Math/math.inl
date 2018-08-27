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
Vector<T> Vector<T>::operator+=(const Vector<T> &v)
{
    this->x = this->x + v.x;
    this->y = this->y + v.y;
    this->z = this->z + v.z;
    return *this;
}

template <typename T>
Vector<T> Vector<T>::operator/=(double s)
{
    double q { 1.0 / s };

    this->x *= q;
    this->y *= q;
    this->z *= q;
    return *this;
}

template <typename T>
Vector<T> Vector<T>::operator*=(double s)
{
    this->x *= s;
    this->y *= s;
    this->z *= s;
    return *this;
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
Vector<T> operator*(const Vector<T> &v1, const Vector<T> &v2)
{
    return Vector<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}


template <typename T>
Vector<T> operator/(const Vector<T> &v, const double scalar)
{
    double q { 1.0 / scalar };
    return Vector<T>(q * v.x, q * v.y, q * v.z);
}

template <typename T>
std::ostream& operator<<(std::ostream &os, const Vector<T> &v)
{
    return os << "(" << v.x << "," << v.y << "," << v.z << ")";
}

template <typename T>
bool operator==(const Vector<T> &v1, const Vector<T> &v2)
{
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

template <typename T>
bool operator!=(const Vector<T> &v1, const Vector<T> &v2)
{
    return !(v1 == v2);
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
void orthonormal_basis(const Vector<T>& v1, Vector<T> &v2, Vector<T> &v3)
{
    v2 = Vec3d(0, -v1.z, v1.y);

    if (v2 == Vector<T>(0, 0, 0))
        v2 = Vec3d(-v1.z, 0, v1.x);

    v3 = v1.cross(v2);
}

}
