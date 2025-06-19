#include "Triangle.h"

#include <eigen3/Eigen/Dense>

Triangle::Triangle(const Polygon& polygon): polygon(polygon)
{
    Eigen::Vector3d v1 = {
        polygon.points[1].x - polygon.points[0].x,
        polygon.points[1].y - polygon.points[0].y,
        polygon.points[1].z - polygon.points[0].z
    };

    Eigen::Vector3d v2 = {
        polygon.points[2].x - polygon.points[0].x,
        polygon.points[2].y - polygon.points[0].y,
        polygon.points[2].z - polygon.points[0].z
    };

    radius = std::max(v1.norm(), v2.norm()) + 1;

    normal = (v1.cross(v2)).normalized();

    vecAB = {
        polygon.points[1].x - polygon.points[0].x,
        polygon.points[1].y - polygon.points[0].y,
        polygon.points[1].z - polygon.points[0].z
    };

    vecAC = {
        polygon.points[2].x - polygon.points[0].x,
        polygon.points[2].y - polygon.points[0].y,
        polygon.points[2].z - polygon.points[0].z
    };

    vecBC = {
        polygon.points[2].x - polygon.points[1].x,
        polygon.points[2].y - polygon.points[1].y,
        polygon.points[2].z - polygon.points[1].z
    };
}

std::vector<Polygon> Triangle::polygons()
{
    return std::vector{polygon};
}

Ray Triangle::getNormalInReflection(Ray& incident)
{
    return getPolygonNormalInReflection(polygon, incident, normal, vecAB, vecAC, vecBC, radius);
}
