//
// Created by anton on 6/2/25.
//

#include "Quadrangle.h"

Quadrangle::Quadrangle(const Point3D& p0, const Point3D& p1, const Point3D& p2, const Point3D& p3): points{p0,p1,p2,p3}
{
}

std::vector<Polygon> Quadrangle::polygons()
{
    std::vector<Polygon> polygons{{points[0],points[1],points[3]}, {points[2],points[3],points[1]}};
    return polygons;
}
