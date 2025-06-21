#ifndef FILLEDRENDERSTRATEGY_H
#define FILLEDRENDERSTRATEGY_H
#include <mutex>

#include "renderStrategy.h"


class FilledRenderStrategy: public RenderStrategy {
public:
    void render(QImage& image, SceneDescription &scene, ConfigState &config, Camera &camera) override;

    int getReadyCnt();

private:
    std::mutex rcMutex;
    int readyCnt = 0;

};



#endif //FILLEDRENDERSTRATEGY_H
