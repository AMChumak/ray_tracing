//
// Created by anton on 6/19/25.
//

#include "RenderArea.h"

#include <QPainter>

RenderArea::RenderArea(QWidget* parent, Camera* camera_, SceneDescription* scene_,
                       ConfigState* config_): camera(camera_), scene(scene_), config(config_)
{
    screen = new QImage(640, 480, QImage::Format_ARGB32);
    wireframeRenderStrategy.render(*screen, *scene, *config, *camera);
}

void RenderArea::paintEvent(QPaintEvent* event)
{

    QPainter painter(this);
    painter.drawImage(QRect(0, 0, screen->width(), screen->height()), *screen);
    painter.end();
    QWidget::paintEvent(event);
}

void RenderArea::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}
