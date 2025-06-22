//
// Created by anton on 6/2/25.
//

#ifndef QUADRANGLE_H
#define QUADRANGLE_H
#include <eigen3/Eigen/Dense>

#include "Figure.h"


class Quadrangle : public Figure
{
public:
    Quadrangle(const Point3D& p0, const Point3D& p1, const Point3D& p2, const Point3D& p3);
    std::vector<Polygon> polygons() override;
    Ray getNormalInReflection(Ray& incident) override;
    std::pair<Point3D, Point3D> getBox() override;

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
