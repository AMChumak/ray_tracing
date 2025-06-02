#include "Sphere.h"
#include <cmath>
#include <valarray>

#define M_SQRT3 1.732050807568877

Sphere::Sphere(const Point3D& center, const double radius): center(center), radius(radius)
{
}


Point3D Sphere::getArcCenter(Point3D a, Point3D b) const
{
    a -= center;
    b -= center;
    Point3D p = (b + a);
    const double normP = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    return (p / normP * radius) + center;
}

void Sphere::bisectPolygon(std::vector<Polygon>& dest, int startIndex, Polygon& source) const
{
    //bisect each side
    Point3D newPoints[3];
    newPoints[0] = getArcCenter(source.points[0], source.points[1]);
    newPoints[1] = getArcCenter(source.points[1], source.points[2]);
    newPoints[2] = getArcCenter(source.points[2], source.points[0]);
    //emplace new polygons
    dest[startIndex] = Polygon{source.points[0], newPoints[0], newPoints[2]};
    dest[startIndex + 1] = Polygon{newPoints[0], source.points[1], newPoints[1]};
    dest[startIndex + 2] = Polygon{newPoints[2], newPoints[1], source.points[2]};
    dest[startIndex + 3] = Polygon{newPoints[1], newPoints[2], newPoints[0]};
}


std::vector<Polygon> Sphere::polygons()
{
    //step 1: create 12-vertex graph (20 triangles)
    Point3D grid[12];
    grid[0] = {center.x, center.y, center.z - radius};

    double smallR = M_SQRT3 * radius / 2;
    grid[1] = {center.x - smallR, center.y, center.z - radius / 2};
    grid[2] = {
        center.x - std::cos(2 * M_PI / 5) * smallR, center.y - std::sin(2 * M_PI / 5) * smallR, center.z - radius / 2
    };
    grid[3] = {
        center.x - std::cos(4 * M_PI / 5) * smallR, center.y - std::sin(4 * M_PI / 5) * smallR, center.z - radius / 2
    };
    grid[4] = {
        center.x - std::cos(6 * M_PI / 5) * smallR, center.y - std::sin(6 * M_PI / 5) * smallR, center.z - radius / 2
    };
    grid[5] = {
        center.x - std::cos(8 * M_PI / 5) * smallR, center.y - std::sin(8 * M_PI / 5) * smallR, center.z - radius / 2
    };

    grid[6] = {center.x + smallR, center.y, center.z + radius / 2};
    grid[7] = {
        center.x + std::cos(2 * M_PI / 5) * smallR, center.y + std::sin(2 * M_PI / 5) * smallR, center.z + radius / 2
    };
    grid[8] = {
        center.x + std::cos(4 * M_PI / 5) * smallR, center.y + std::sin(4 * M_PI / 5) * smallR, center.z + radius / 2
    };
    grid[9] = {
        center.x + std::cos(6 * M_PI / 5) * smallR, center.y + std::sin(6 * M_PI / 5) * smallR, center.z + radius / 2
    };
    grid[10] = {
        center.x + std::cos(8 * M_PI / 5) * smallR, center.y + std::sin(8 * M_PI / 5) * smallR, center.z + radius / 2
    };

    grid[11] = {center.x, center.y, center.z + radius};


    const int maxsize = 20 << (accuracy * 2);
    std::vector<Polygon> polygons[2];
    polygons[0].resize(maxsize);
    polygons[1].resize(maxsize);

    //init
    polygons[0][0] = Polygon{grid[0], grid[2], grid[1]};
    polygons[0][1] = Polygon{grid[0], grid[3], grid[2]};
    polygons[0][2] = Polygon{grid[0], grid[4], grid[3]};
    polygons[0][3] = Polygon{grid[0], grid[5], grid[4]};
    polygons[0][4] = Polygon{grid[0], grid[1], grid[5]};

    polygons[0][5] = Polygon{grid[1], grid[2], grid[9]};
    polygons[0][6] = Polygon{grid[2], grid[3], grid[10]};
    polygons[0][7] = Polygon{grid[3], grid[4], grid[6]};
    polygons[0][8] = Polygon{grid[4], grid[5], grid[7]};
    polygons[0][9] = Polygon{grid[5], grid[1], grid[8]};

    polygons[0][10] = Polygon{grid[9], grid[8], grid[1]};
    polygons[0][11] = Polygon{grid[10], grid[9], grid[2]};
    polygons[0][12] = Polygon{grid[6], grid[10], grid[3]};
    polygons[0][13] = Polygon{grid[7], grid[6], grid[4]};
    polygons[0][14] = Polygon{grid[8], grid[7], grid[5]};

    polygons[0][15] = Polygon{grid[6], grid[7], grid[11]};
    polygons[0][16] = Polygon{grid[7], grid[8], grid[11]};
    polygons[0][17] = Polygon{grid[8], grid[9], grid[11]};
    polygons[0][18] = Polygon{grid[9], grid[10], grid[11]};
    polygons[0][19] = Polygon{grid[10], grid[6], grid[11]};

    for (int i = 1; i <= accuracy; i++)
    {
        auto &cur = polygons[i % 2];
        auto &inv = polygons[(i + 1) % 2];

        //bisection of sphere triangle
        int j = 0;
        for (auto &polygon : inv)
        {
            bisectPolygon(cur, j, polygon);
            j += 4;
        }
    }
    return polygons[accuracy % 2];
}
