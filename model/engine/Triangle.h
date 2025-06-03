#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <Eigen/src/Core/Matrix.h>

#include "Figure.h"


class Triangle : public Figure
{
    public:
    explicit Triangle(const Polygon& polygon);
    std::vector<Polygon> polygons() override;
    Ray getNormalInReflection(const Ray& incident) override;

private:
    Polygon polygon;
    double radius;
    Eigen::Vector3d normal;
    Eigen::Vector3d vecAB, vecAC, vecBC;
};


#endif //TRIANGLE_H
