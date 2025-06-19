#ifndef FILLEDRENDERSTRATEGY_H
#define FILLEDRENDERSTRATEGY_H
#include "renderStrategy.h"


class FilledRenderStrategy: public RenderStrategy {
public:
    void render(QImage& image, SceneDescription &scene, ConfigState &config, Camera &camera) override;
};



#endif //FILLEDRENDERSTRATEGY_H
