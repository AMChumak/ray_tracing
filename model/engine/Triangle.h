#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Figure.h"


class Triangle : public Figure
{
    public:
    explicit Triangle(const Polygon& polygon);
    std::vector<Polygon> polygons() override;
private:
    Polygon polygon;
};


#endif //TRIANGLE_H
