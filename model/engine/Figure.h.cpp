
#include "Figure.h"


Point3D& Point3D::operator+=(const Point3D& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Point3D Point3D::operator+(const Point3D& other) const
{
    return Point3D(*this) += other;
}

Point3D& Point3D::operator-=(const Point3D& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Point3D Point3D::operator-(const Point3D& other) const
{
    return Point3D(*this) -= other;
}

Point3D& Point3D::operator*=(const double q)
{
    x *= q;
    y *= q;
    z *= q;
    return *this;
}

Point3D& Point3D::operator*(const double q) const
{
    return Point3D(*this) *= q;
}

Point3D& Point3D::operator/=(const double q)
{
    x /= q;
    y /= q;
    z /= q;
    return *this;
}

Point3D& Point3D::operator/(const double q) const
{
    return Point3D(*this) /= q;
}
