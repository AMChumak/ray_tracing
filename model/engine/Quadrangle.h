//
// Created by anton on 6/2/25.
//

#ifndef QUADRANGLE_H
#define QUADRANGLE_H
#include "Figure.h"


class Quadrangle : public Figure
{
public:
    Quadrangle(const Point3D& p0, const Point3D& p1, const Point3D& p2, const Point3D& p3);
    std::vector<Polygon> polygons() override;

    private:
    Point3D points[4];
};


#endif //QUADRANGLE_H
