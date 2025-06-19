#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <eigen3/Eigen/Dense>

#include "Figure.h"


class Triangle : public Figure
{
    public:
    explicit Triangle(const Polygon& polygon);
    std::vector<Polygon> polygons() override;
    Ray getNormalInReflection(Ray& incident) override;

private:
    Polygon polygon;
    double radius;
    Eigen::Vector3d normal;
    Eigen::Vector3d vecAB, vecAC, vecBC;
};


#endif //TRIANGLE_H
