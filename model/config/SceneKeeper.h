//
// Created by anton on 6/3/25.
//

#ifndef SCENEKEEPER_H
#define SCENEKEEPER_H

#include <string>

#include "Figure.h"
#include "optics.h"

struct SceneDescription
{
    SceneProps props;
    std::vector<Figure> figures;
    std::vector<FigureOpticProps> optics;
};

class SceneKeeper
{
public:
    SceneDescription getSceneDescription(std::string fileName);
};


#endif //SCENEKEEPER_H
