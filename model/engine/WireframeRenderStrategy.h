//
// Created by anton on 6/3/25.
//

#ifndef WIREFRAMERENDERSTRATEGY_H
#define WIREFRAMERENDERSTRATEGY_H

#include "renderStrategy.h"


struct Line
{
    Point3D begin;
    Point3D end;
    QColor color;
};

class WireframeRenderStrategy: public RenderStrategy {
public:
    void render(QImage& image, SceneDescription &scene, ConfigState &config, Camera &camera) override;
};



#endif //WIREFRAMERENDERSTRATEGY_H
