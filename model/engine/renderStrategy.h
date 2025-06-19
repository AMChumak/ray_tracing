#ifndef RENDERSTRATEGY_H
#define RENDERSTRATEGY_H

#include "optics.h"
#include <QImage>

#include "Camera.h"
#include "ConfigKeeper.h"

class RenderStrategy
{
public:
    virtual ~RenderStrategy() = default;
    virtual void render(QImage& image, SceneDescription &scene, ConfigState &config, Camera &camera) = 0;
};


#endif //RENDERSTRATEGY_H
