
#include "Triangle.h"

Triangle::Triangle(const Polygon& polygon): polygon(polygon)
{
}

std::vector<Polygon> Triangle::polygons()
{
    return std::vector{polygon};
}
