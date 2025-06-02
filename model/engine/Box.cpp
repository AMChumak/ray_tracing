//
// Created by anton on 6/2/25.
//

#include "Box.h"

Box::Box(const Point3D& min, const Point3D& max): minP(min), maxP(max)
{
}

std::vector<Polygon> Box::polygons()
{
    std::vector<Polygon> polygons{12};
    polygons[0] = {minP, {maxP.x, minP.y, minP.z}, {minP.x, minP.y, maxP.z}};
    polygons[1] = {Point3D{minP.x, minP.y, maxP.z}, {maxP.x, minP.y, minP.z}, {maxP.x, minP.y, maxP.z}};

    polygons[2] = {minP, {minP.x, minP.y, maxP.z}, {minP.x, maxP.y, minP.z}};
    polygons[3] = {Point3D{minP.x, minP.y, maxP.z}, {minP.x, maxP.y, maxP.z}, {minP.x, maxP.y, minP.z}};

    polygons[4] = {minP, {minP.x, maxP.y, minP.z}, {maxP.x, minP.y, minP.z}};
    polygons[5] = {Point3D{maxP.x, maxP.y, minP.z}, {maxP.x, minP.y, minP.z}, {minP.x, maxP.y, minP.z}};

    polygons[6] = {maxP, {minP.x, maxP.y, maxP.z}, {maxP.x, minP.y, maxP.z}};
    polygons[7] = {Point3D{minP.x, minP.y, maxP.z}, {maxP.x, minP.y, maxP.z}, {minP.x, maxP.y, maxP.z}};

    polygons[8] = {maxP, {maxP.x, minP.y, maxP.z}, {maxP.x, maxP.y, minP.z}};
    polygons[9] = {Point3D{maxP.x, minP.y, minP.z}, {maxP.x, maxP.y, minP.z}, {maxP.x, minP.y, maxP.z}};

    polygons[10] = {maxP, {maxP.x, maxP.y, minP.z}, {minP.x, maxP.y, maxP.z}};
    polygons[11] = {Point3D{minP.x, maxP.y, minP.z}, {minP.x, maxP.y, maxP.z}, {maxP.x, maxP.y, minP.z}};

    return polygons;
}
