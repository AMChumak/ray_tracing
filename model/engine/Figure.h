#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include <eigen3/Eigen/Dense>

struct Point3D
{
    double x;
    double y;
    double z;

    Point3D& operator +=(const Point3D& other);
    Point3D operator +(const Point3D& other) const;
    Point3D& operator -=(const Point3D& other);
    Point3D operator -(const Point3D& other) const;
    Point3D& operator*=(double q);
    Point3D& operator*(double q) const;
    Point3D& operator/=(double q);
    Point3D& operator/(double q) const;
    bool operator!=(const Point3D& point3D) const;
    bool operator==(const Point3D& point3D) const;
};

struct Ray
{
    Point3D origin;
    Point3D direction;
};


struct Polygon
{
    Point3D points[3];
};

Ray getPolygonNormalInReflection(const Polygon &polygon, Ray& incident, Eigen::Vector3d& normal, Eigen::Vector3d& AB, Eigen::Vector3d& AC,
                          Eigen::Vector3d& BC, double radius = 1000000000.0);

class Figure
{
public:
    virtual ~Figure() = default;
    virtual std::vector<Polygon> polygons() = 0;
    virtual Ray getNormalInReflection(Ray& incident) = 0;
};


#endif //FIGURE_H
