//
// Created by anton on 6/2/25.
//

#ifndef QUADRANGLE_H
#define QUADRANGLE_H
#include <Eigen/src/Core/Matrix.h>

#include "Figure.h"


class Quadrangle : public Figure
{
public:
    Quadrangle(const Point3D& p0, const Point3D& p1, const Point3D& p2, const Point3D& p3);
    std::vector<Polygon> polygons() override;
    Ray getNormalInReflection(const Ray& incident) override;

private:
    Point3D points[4];
    double radius1;
    double radius2;
    Eigen::Vector3d normal1;
    Eigen::Vector3d normal2;
    Eigen::Vector3d vecAB, vecAC, vecBC;
    Eigen::Vector3d vecDE, vecDF, vecEF;
};


#endif //QUADRANGLE_H
