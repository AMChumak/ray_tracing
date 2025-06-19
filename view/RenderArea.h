//
// Created by anton on 6/19/25.
//

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

#include "Camera.h"
#include "optics.h"

#include "FilledRenderStrategy.h"
#include "WireframeRenderStrategy.h"

class RenderArea final : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent, Camera *camera_, SceneDescription *scene_, ConfigState *config_);

protected slots:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QImage *screen;
    Camera *camera;
    SceneDescription *scene;
    ConfigState *config;
    FilledRenderStrategy filledRenderStrategy;
    WireframeRenderStrategy wireframeRenderStrategy;
};


#endif //RENDERAREA_H
