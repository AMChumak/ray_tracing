#ifndef OPTICS_H
#define OPTICS_H

#include "Figure.h"


struct LightSource
{
    Point3D position;
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct SceneProps
{
    unsigned char ar;
    unsigned char ag;
    unsigned char ab;
    std::vector<LightSource> lights;
};

struct FigureOpticProps
{
    // коэффициенты диффузного и рассеянного (одни и те же) отражения для красной зелёной и синей составляющей света
    double kdr;
    double kdg;
    double kdb;
    // коэффициенты зеркального отражения для красной, зелёной и синей составляющей света
    double ksr;
    double ksg;
    double ksb;
    // показатель зеркальности по Блинну
    double power;
};


#endif //OPTICS_H
