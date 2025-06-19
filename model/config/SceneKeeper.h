//
// Created by anton on 6/3/25.
//

#ifndef SCENEKEEPER_H
#define SCENEKEEPER_H

#include <string>

#include "optics.h"



class SceneKeeper
{
public:
    SceneDescription getSceneDescription(std::string fileName);
};


#endif //SCENEKEEPER_H
