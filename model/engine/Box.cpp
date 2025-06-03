//
// Created by anton on 6/2/25.
//

#include "Box.h"
#include "Figure.h"

#include <oneapi/tbb/task_arena.h>

Box::Box(const Point3D& min, const Point3D& max): minP(min), maxP(max)
{
    triangles.resize(12);
    sides.resize(12);
    normals.resize(12);
    radii.resize(12);

    triangles[0] = {minP, {maxP.x, minP.y, minP.z}, {minP.x, minP.y, maxP.z}};
    triangles[1] = {Point3D{minP.x, minP.y, maxP.z}, {maxP.x, minP.y, minP.z}, {maxP.x, minP.y, maxP.z}};

    triangles[2] = {minP, {minP.x, minP.y, maxP.z}, {minP.x, maxP.y, minP.z}};
    triangles[3] = {Point3D{minP.x, minP.y, maxP.z}, {minP.x, maxP.y, maxP.z}, {minP.x, maxP.y, minP.z}};

    triangles[4] = {minP, {minP.x, maxP.y, minP.z}, {maxP.x, minP.y, minP.z}};
    triangles[5] = {Point3D{maxP.x, maxP.y, minP.z}, {maxP.x, minP.y, minP.z}, {minP.x, maxP.y, minP.z}};

    triangles[6] = {maxP, {minP.x, maxP.y, maxP.z}, {maxP.x, minP.y, maxP.z}};
    triangles[7] = {Point3D{minP.x, minP.y, maxP.z}, {maxP.x, minP.y, maxP.z}, {minP.x, maxP.y, maxP.z}};

    triangles[8] = {maxP, {maxP.x, minP.y, maxP.z}, {maxP.x, maxP.y, minP.z}};
    triangles[9] = {Point3D{maxP.x, minP.y, minP.z}, {maxP.x, maxP.y, minP.z}, {maxP.x, minP.y, maxP.z}};

    triangles[10] = {maxP, {maxP.x, maxP.y, minP.z}, {minP.x, maxP.y, maxP.z}};
    triangles[11] = {Point3D{minP.x, maxP.y, minP.z}, {minP.x, maxP.y, maxP.z}, {maxP.x, maxP.y, minP.z}};


    for (int i = 0; i < 12; i++)
    {
        auto &polygon = triangles[i].points;
        sides[i].resize(3);
        sides[i][0] = {polygon[1].x - polygon[0].x, polygon[1].y - polygon[0].y, polygon[1].z - polygon[0].z}; // AB
        sides[i][1] = {polygon[2].x - polygon[0].x, polygon[2].y - polygon[0].y, polygon[2].z - polygon[0].z}; // AC
        sides[i][2] = {polygon[2].x - polygon[1].x, polygon[2].y - polygon[1].y, polygon[2].z - polygon[1].z}; // BC

        radii[i] = std::max(sides[i][0].norm(), sides[i][2].norm()) + 1;

        normals[i] = (sides[i][0].cross(sides[i][2])).normalized();
    }
}

std::vector<Polygon> Box::polygons()
{
    return triangles;
}

Ray Box::getNormalInReflection(const Ray& incident)
{
    Ray normal{};
    for (int i = 0; i < 12; i++)
    {
        normal = getPolygonNormalInReflection(triangles[i], incident, normals[i], sides[i][0], sides[i][1], sides[i][2],radii[i]);
        if (normal.direction != Point3D{0,0,0})
            return normal;
    }
    return normal;
}
