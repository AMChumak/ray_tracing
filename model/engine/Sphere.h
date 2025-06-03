//
// Created by anton on 6/2/25.
//

#ifndef SPHERE_H
#define SPHERE_H
#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/Core/functors/TernaryFunctors.h>

#include "Figure.h"


class Sphere : public Figure
{
public:
    Sphere(const Point3D& center, double radius);

    std::vector<Polygon> polygons() override;

private:
    [[nodiscard]] Point3D getArcCenter(Point3D a, Point3D b) const;
    void bisectPolygon(std::vector<Polygon>& dest, int startIndex, Polygon& source) const;

public:
    Ray getNormalInReflection(const Ray& incident) override;

private:
    Eigen::Vector3d center;
    double radius;
    int accuracy = 1;
};


#endif //SPHERE_H
