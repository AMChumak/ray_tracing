//
// Created by anton on 6/2/25.
//

#ifndef BOX_H
#define BOX_H
#include <Eigen/src/Core/Matrix.h>

#include "Figure.h"


class Box : public Figure
{
public:
    Box(const Point3D& min, const Point3D& max);
    std::vector<Polygon> polygons() override;
    Ray getNormalInReflection(const Ray& incident) override;

private:
    std::vector<Polygon> triangles;
    std::vector<Eigen::Vector3d> normals;
    std::vector<std::vector<Eigen::Vector3d>> sides;
    std::vector<double> radii;
    Point3D minP;
    Point3D maxP;


};


#endif //BOX_H
