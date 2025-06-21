#ifndef CONFIGKEEPER_H
#define CONFIGKEEPER_H

#include <string>
#include "Figure.h"

struct ConfigState
{
    int br;
    int bg;
    int bb;
    double gamma;
    int depth;
    int quality;
    Point3D eye;
    Point3D view;
    Point3D up;
    double zf;
    double zb;
    double sw;
    double sh;
};


class ConfigKeeper
{
public:
    ConfigKeeper();
    std::string readConfig(const std::string& configFile);
    std::string writeConfig(const std::string& configFile) const;

public:
    ConfigState state{};
};


#endif //CONFIGKEEPER_H
