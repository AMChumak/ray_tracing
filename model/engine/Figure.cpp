
#include <eigen3/Eigen/Dense>

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

bool Point3D::operator!=(const Point3D& point3D) const
{
    return !(*this == point3D);
}

bool Point3D::operator==(const Point3D& point3D) const
{
    return x == point3D.x && y == point3D.y && z == point3D.z;
}

Ray getPolygonNormalInReflection(const Polygon &polygon, Ray& incident, Eigen::Vector3d& normal, Eigen::Vector3d& vecAB, Eigen::Vector3d& vecAC,
                                 Eigen::Vector3d& vecBC, double radius)
{
    //find quotient needed to set point on plane

    Eigen::Vector3d incidentV{
        incident.direction.x, incident.direction.y,
        incident.direction.z
    };
    Eigen::Vector3d originV{incident.origin.x, incident.origin.y, incident.origin.z};

    if (incidentV.dot(normal) >= 0)
    {
        return Ray{}; //there is no intersection
    }

    Eigen::Vector3d vectorA{polygon.points[0].x, polygon.points[0].y, polygon.points[0].z};

    Eigen::Vector3d tmp = vectorA - originV;
    double q = tmp.dot(normal) / incidentV.dot(normal);

    if (q < 0)
    {
        return Ray{};
    }

    Eigen::Vector3d intersectionV = q * incidentV + originV;
    //check that point in triangle

    Eigen::Vector3d vecAI = intersectionV - vectorA;

    if (vecAI.norm() > radius)
    {
        return Ray{};
    }


    //check AB border

    Eigen::Vector3d pr = vecAB * vecAI.dot(vecAB) / vecAB.dot(vecAB);
    Eigen::Vector3d rest = vecAI - pr;

    if (rest.dot(vecAC) <= 0)
    {
        return Ray{};
    }

    //check AC border
    pr = vecAC * vecAI.dot(vecAC) / vecAC.dot(vecAC);
    rest = vecAI - pr;
    if (rest.dot(vecAB) <= 0)
    {
        return Ray{};
    }
    //check BC border
    Eigen::Vector3d vecBI = intersectionV - Eigen::Vector3d{
        polygon.points[1].x, polygon.points[1].y, polygon.points[1].z
    };
    pr = vecBC * vecBI.dot(vecBC) / vecBC.dot(vecBC);
    rest = vecBI - pr;
    if (rest.dot(-vecAB) <= 0)
    {
        return Ray{};
    }

    return Ray{{intersectionV.x(), intersectionV.y(), intersectionV.z()}, {normal.x(), normal.y(), normal.z()}};
}
