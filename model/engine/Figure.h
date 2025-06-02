#ifndef FIGURE_H
#define FIGURE_H

#include <vector>

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

class Figure
{
public:
    virtual ~Figure() = default;
    virtual std::vector<Polygon> polygons() = 0;
};


#endif //FIGURE_H
