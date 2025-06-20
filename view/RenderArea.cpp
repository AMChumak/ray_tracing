//
// Created by anton on 6/19/25.
//

#include "RenderArea.h"

#include <iostream>
#include <qevent.h>
#include <QPainter>

RenderArea::RenderArea(QWidget* parent, Camera* camera_, SceneDescription* scene_,
                       ConfigState* config_): camera(camera_), scene(scene_), config(config_)
{
    connect(this, &RenderArea::requestUpdate, this, &RenderArea::onRequestUpdate, Qt::QueuedConnection);

    setFocus();
    screen = new QImage(640, 480, QImage::Format_ARGB32);
    wireRenderThreadRunning = true;
    wireRenderThread = new std::thread(&RenderArea::renderWireframes, this);
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

void RenderArea::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W)
    {
        if (keyWPressed)
            return;
        keyWPressed = true;
        std::lock_guard sguard(stepM);
        moveStep += Point3D{0, 0.1, 0};
    }
    if (event->key() == Qt::Key_A)
    {
        if (keyAPressed)
            return;
        keyAPressed = true;
        std::lock_guard sguard(stepM);
        moveStep += Point3D{-0.1, 0, 0};
    }
    if (event->key() == Qt::Key_S)
    {
        if (keySPressed)
            return;
        keySPressed = true;
        std::lock_guard sguard(stepM);
        moveStep += Point3D{0, -0.1, 0};
    }
    if (event->key() == Qt::Key_D)
    {
        if (keyDPressed)
            return;
        keyDPressed = true;
        std::lock_guard sguard(stepM);
        moveStep += Point3D{0.1, 0, 0};
    }
    if (event->key() == Qt::Key_Control)
    {
        keyCtrlPressed = true;
    }
    if (event->key() == Qt::Key_Shift)
    {
        keyShiftPressed = true;
    }
    QWidget::keyPressEvent(event);
}

void RenderArea::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W)
    {
        keyWPressed = false;
        std::lock_guard sguard(stepM);
        moveStep -= Point3D{0, 0.1, 0};
    }
    if (event->key() == Qt::Key_A)
    {
        keyAPressed = false;
        std::lock_guard sguard(stepM);
        moveStep -= Point3D{-0.1, 0, 0};
    }
    if (event->key() == Qt::Key_S)
    {
        keySPressed = false;
        std::lock_guard sguard(stepM);
        moveStep -= Point3D{0, -0.1, 0};
    }
    if (event->key() == Qt::Key_D)
    {
        keyDPressed = false;
        std::lock_guard sguard(stepM);
        moveStep -= Point3D{0.1, 0, 0};
    }
    if (event->key() == Qt::Key_Control)
    {
        keyCtrlPressed = false;
    }
    if (event->key() == Qt::Key_Shift)
    {
        keyShiftPressed = false;
    }
    QWidget::keyReleaseEvent(event);
}

void RenderArea::wheelEvent(QWheelEvent* event)
{
    {
        std::lock_guard sguard(cameraM);
        if (keyCtrlPressed)
        {
            if (event->angleDelta().y() > 0)
                camera->zoom(1.1);
            else
                camera->zoom(0.9);
        }
        else if (keyShiftPressed)
        {
            if (event->angleDelta().y() > 0)
                camera->rotateAroundUp(1.0);
            else
                camera->rotateAroundUp(-1.0);
        }
        else
        {
            if (event->angleDelta().y() > 0)
                camera->rotateAroundX(1.0);
            else
                camera->rotateAroundX(-1.0);
        }
    }
    QWidget::wheelEvent(event);
}

void RenderArea::renderWireframes()
{
    while (wireRenderThreadRunning)
    {
        //work with camera
        {
            std::lock_guard cguard(cameraM);

            //check move
            {
                std::lock_guard sguard(stepM);
                if (moveStep != Point3D{})
                {
                    camera->move(moveStep);
                }
            }

            //draw render
            wireframeRenderStrategy.render(*screen, *scene, *config, *camera);
        }
        emit requestUpdate();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
