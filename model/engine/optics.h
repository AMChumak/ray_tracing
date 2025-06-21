#ifndef OPTICS_H
#define OPTICS_H

#include "Figure.h"



struct LightSource
{
    Point3D position;
    int r;
    int g;
    int b;
};

struct SceneProps
{
    int ar;
    int ag;
    int ab;
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

struct SceneDescription
{
    SceneProps props;
    std::vector<Figure*> figures;
    std::vector<FigureOpticProps> optics;
};

#endif //OPTICS_H
