//
// Created by anton on 6/19/25.
//

#include "RenderArea.h"

#include <iostream>
#include <qcolumnview.h>
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

    std::cout << isRenderShowing << std::endl;
    if (isRenderShowing)
    {
        painter.drawImage(QRect(0, 0, render->width(), render->height()), *render);
    }
    else
    {
        painter.drawImage(QRect(0, 0, screen->width(), screen->height()), *screen);

        if (progress)
        {
            double percent = progress / (screen->width() * screen->height()) * 100;
            if (percent < 34)
                painter.setPen(Qt::red);
            else if (percent < 67)
                painter.setPen(Qt::yellow);
            else
                painter.setPen(Qt::green);
            auto progressMsg = QString("%1%").arg(static_cast<int>(percent));
            painter.setFont(QFont("Arial", 40));
            painter.drawText(QPointF(50, 50), progressMsg);
        }
    }

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
        if (keyWPressed || renderInProcess || isRenderShowing)
            return;
        keyWPressed = true;
        std::lock_guard sguard(stepM);
        moveStep += Point3D{0, 0.1, 0};
    }
    if (event->key() == Qt::Key_A)
    {
        if (keyAPressed || renderInProcess || isRenderShowing)
            return;
        keyAPressed = true;
        std::lock_guard sguard(stepM);
        moveStep += Point3D{0.1, 0, 0};
    }
    if (event->key() == Qt::Key_S)
    {
        if (keySPressed || renderInProcess || isRenderShowing)
            return;
        keySPressed = true;
        std::lock_guard sguard(stepM);
        moveStep += Point3D{0, -0.1, 0};
    }
    if (event->key() == Qt::Key_D)
    {
        if (keyDPressed || renderInProcess || isRenderShowing)
            return;
        keyDPressed = true;
        std::lock_guard sguard(stepM);
        moveStep += Point3D{-0.1, 0, 0};
    }
    if (event->key() == Qt::Key_Control)
    {
        keyCtrlPressed = true;
    }
    if (event->key() == Qt::Key_Shift)
    {
        keyShiftPressed = true;
    }
    if (event->key() == Qt::Key_Enter)
    {
        if (isRenderShowing)
        {
            isRenderShowing = false;
            return;
        }
        if (renderInProcess)
            return;

        renderInProcess = true;
        rtRenderStart();
    }
    QWidget::keyPressEvent(event);
}

void RenderArea::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W)
    {
        if (renderInProcess || isRenderShowing)
            return;
        keyWPressed = false;
        std::lock_guard sguard(stepM);
        moveStep -= Point3D{0, 0.1, 0};
    }
    if (event->key() == Qt::Key_A)
    {
        if (renderInProcess || isRenderShowing)
            return;
        keyAPressed = false;
        std::lock_guard sguard(stepM);
        moveStep -= Point3D{0.1, 0, 0};
    }
    if (event->key() == Qt::Key_S)
    {
        if (renderInProcess || isRenderShowing)
            return;
        keySPressed = false;
        std::lock_guard sguard(stepM);
        moveStep -= Point3D{0, -0.1, 0};
    }
    if (event->key() == Qt::Key_D)
    {
        if (renderInProcess || isRenderShowing)
            return;
        keyDPressed = false;
        std::lock_guard sguard(stepM);
        moveStep -= Point3D{-0.1, 0, 0};
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
                camera->rotateAroundUp(-1.0);
            else
                camera->rotateAroundUp(1.0);
        }
        else
        {
            if (event->angleDelta().y() > 0)
                camera->rotateAroundX(-1.0);
            else
                camera->rotateAroundX(1.0);
        }
    }
    QWidget::wheelEvent(event);
}

void RenderArea::rtRenderStart()
{
    rtRenderThread = new std::thread(&RenderArea::renderRayTracing, this);
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
        progress = filledRenderStrategy.getReadyCnt();
        emit requestUpdate();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void RenderArea::renderRayTracing()
{
    delete render;
    render = new QImage(screen->width(), screen->height(), QImage::Format_ARGB32);
    filledRenderStrategy.render(*render, *scene, *config, *camera);
    renderInProcess = false;
    isRenderShowing = true;
}
