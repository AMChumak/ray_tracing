//
// Created by anton on 6/2/25.
//

#include "Quadrangle.h"

Quadrangle::Quadrangle(const Point3D& p0, const Point3D& p1, const Point3D& p2, const Point3D& p3): points{
    p0, p1, p2, p3
}
{
    //for first plane
    Eigen::Vector3d v1 = {
        points[1].x - points[0].x,
        points[1].y - points[0].y,
        points[1].z - points[0].z
    };

    Eigen::Vector3d v2 = {
        points[3].x - points[0].x,
        points[3].y - points[0].y,
        points[3].z - points[0].z
    };

    radius1 = std::max(v1.norm(), v2.norm()) + 1;

    normal1 = (v1.cross(v2)).normalized();

    //for second plane (maybe it is the same plane)

    Eigen::Vector3d v3 = {
        points[2].x - points[1].x,
        points[2].y - points[1].y,
        points[2].z - points[1].z
    };

    Eigen::Vector3d v4 = {
        points[3].x - points[1].x,
        points[3].y - points[1].y,
        points[3].z - points[1].z
    };

    radius2 = std::max(v3.norm(), v4.norm()) + 1;

    normal2 = (v3.cross(v4)).normalized();

    //for first plane
    vecAB = {
        points[1].x - points[0].x,
        points[1].y - points[0].y,
        points[1].z - points[0].z
    };

    vecAC = {
        points[3].x - points[0].x,
        points[3].y - points[0].y,
        points[3].z - points[0].z
    };

    vecBC = {
        points[3].x - points[1].x,
        points[3].y - points[1].y,
        points[3].z - points[1].z
    };

    //for second plane

    vecDE = {
        points[2].x - points[1].x,
        points[2].y - points[1].y,
        points[2].z - points[1].z
    };

    vecDF = {
        points[3].x - points[1].x,
        points[3].y - points[1].y,
        points[3].z - points[1].z
    };

    vecEF = {
        points[3].x - points[2].x,
        points[3].y - points[2].y,
        points[3].z - points[2].z
    };
}

std::vector<Polygon> Quadrangle::polygons()
{
    std::vector<Polygon> polygons{{points[0], points[1], points[3]}, {points[2], points[3], points[1]}};
    return polygons;
}

Ray Quadrangle::getNormalInReflection(const Ray& incident)
{
    Polygon first{points[0], points[1], points[3]};
    Polygon second{points[1], points[2], points[3]};
    Ray normal = getPolygonNormalInReflection(first, incident, normal1, vecAB, vecAC, vecBC, radius1);

    if (normal.direction != Point3D{0,0,0})
        return normal;

    normal = getPolygonNormalInReflection(second, incident, normal2, vecDE, vecDF, vecEF, radius2);
    return normal;
}
