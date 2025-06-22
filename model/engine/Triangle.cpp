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

std::pair<Point3D, Point3D> Triangle::getBox()
{
    Point3D min = polygon.points[0];
    Point3D max = polygon.points[0];

    for (int i = 1; i < 3; i++)
    {
        auto &point = polygon.points[i];
        if (point.x < min.x)
            min.x = point.x;
        if (point.x > max.x)
            max.x = point.x;
        if (point.y < min.y)
            min.y = point.y;
        if (point.y > max.y)
            max.y = point.y;
        if (point.z < min.z)
            min.z = point.z;
        if (point.z > max.z)
            max.z = point.z;
    }
    Point3D diff{1,1,1};
    min -= diff;
    max += diff;
    return std::make_pair(min, max);
}
