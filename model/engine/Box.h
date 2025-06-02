//
// Created by anton on 6/2/25.
//

#ifndef BOX_H
#define BOX_H
#include "Figure.h"


class Box : public Figure
{
public:
    Box(const Point3D& min, const Point3D& max);
    std::vector<Polygon> polygons() override;
    private:
    Point3D minP;
    Point3D maxP;
};


#endif //BOX_H
