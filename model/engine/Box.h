//
// Created by anton on 6/2/25.
//

#ifndef BOX_H
#define BOX_H
#include <eigen3/Eigen/Dense>

#include "Figure.h"


class Box : public Figure
{
public:
    Box(const Point3D& min, const Point3D& max);
    std::vector<Polygon> polygons() override;
    Ray getNormalInReflection(Ray& incident) override;
    std::pair<Point3D, Point3D> getBox() override;

private:
    std::vector<Polygon> triangles;
    std::vector<Eigen::Vector3d> normals;
    std::vector<std::vector<Eigen::Vector3d>> sides;
    std::vector<double> radii;
    Point3D minP;
    Point3D maxP;


};


#endif //BOX_H
