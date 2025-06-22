//
// Created by anton on 6/19/25.
//

#include "RenderArea.h"

#include <iostream>
#include <qcolumnview.h>
#include <qevent.h>
#include <QFile>
#include <QPainter>

#ifndef M_SQRT3
#define M_SQRT3 1.732050807568877
#endif

RenderArea::RenderArea(QWidget* parent, Camera* camera_, SceneDescription* scene_,
                       ConfigState* config_): camera(camera_), scene(scene_), config(config_)
{
    connect(this, &RenderArea::requestUpdate, this, &RenderArea::onRequestUpdate, Qt::QueuedConnection);

    setFocus();
    setFocusPolicy(Qt::StrongFocus);
    screen = new QImage(this->width(), this->height(), QImage::Format_ARGB32);
    wireRenderThreadRunning = true;
    wireRenderThread = new std::thread(&RenderArea::renderWireframes, this);
}

void RenderArea::saveRender(const QString& fileName) const
{
    if (render)
    {
        QFile file(fileName);
        render->save(&file);
    }
}

void RenderArea::saveConfig(const QString& fileName)
{
    ConfigKeeper keeper;
    {
        std::lock_guard guard(cameraM);
        keeper.state = *config;
        keeper.state.eye = camera->getPosition();
        keeper.state.view = camera->getViewPoint();
        keeper.state.up = camera->getUpVector();
        keeper.state.zf = camera->getZf();
        keeper.state.zb = camera->getZb();
    }
    keeper.writeConfig(fileName.toStdString());
}

void RenderArea::setConfig(const ConfigState& config)
{
    std::lock_guard guard(cameraM);
    *this->config = config;
    *camera = Camera{
        config.eye, config.view, config.up, config.zf,
        config.zb, config.sw, config.sh
    };
    isConfigLoaded = true;
}

void RenderArea::loadScene(SceneDescription &scene_, const ConfigState &config_, const bool &isConfigLoaded)
{
    {
        std::lock_guard guard(cameraM);
        scene->figures = scene_.figures;
        scene->optics = scene_.optics;
        scene->props.lights  = scene_.props.lights;
        scene->props.ab = scene_.props.ab;
        scene->props.ag = scene_.props.ag;
        scene->props.ar = scene_.props.ar;
        *config = config_;
        this->isConfigLoaded = isConfigLoaded;
    }
    initCameraPosition();
}

void RenderArea::initCameraPosition()
{
    if (isConfigLoaded)
    {
        std::lock_guard guard(cameraM);
        *camera = Camera{
            config->eye, config->view, config->up, config->zf,
            config->zb, config->sw, config->sh
        };
        return;
    }
    //if there is no config file, use standard configuration
    initCameraWithoutConfig();
}

void RenderArea::onChangeRenderMode(const bool& mode)
{
    if (mode && !isRenderShowing)
    {
        rtRenderStart();
    }
    else if (!mode && isRenderShowing)
    {
        if (renderInProcess)
            return;
        isRenderShowing = false;
        emit changedRenderMode(false);
    }
}

void RenderArea::onBackgroundColorChanged(const QColor& color)
{
    std::lock_guard guard(cameraM);
    config->br = color.red();
    config->bg = color.green();
    config->bb = color.blue();
}

void RenderArea::onGammaChanged(const double& gamma)
{
    std::lock_guard guard(cameraM);
    config->gamma = gamma;
}

void RenderArea::onTracingDepthChanged(const int& depth)
{
    std::lock_guard guard(cameraM);
    config->depth = depth;
}

void RenderArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    if (isRenderShowing)
    {
        painter.drawImage(QRect(0, 0, render->width(), render->height()), *render);
    }
    else
    {
        painter.drawImage(QRect(0, 0, screen->width(), screen->height()), *screen);

        if (progress)
        {
            if (config->bb > 128 || config->bg > 128 || config->br > 128)
                painter.setPen(Qt::black);
            else
                painter.setPen(Qt::white);
            auto progressMsg = QString("rendering...");
            painter.setFont(QFont("Arial", 20));
            painter.drawText(QPointF(84, 50), progressMsg);
            double percent = static_cast<double>(progress) / (screen->width() * screen->height()) * 100;
            QColor color = Qt::red;
            if (percent < 34)
                color = Qt::red;
            else if (percent < 67)
                color = Qt::yellow;
            else
                color = Qt::green;
            painter.fillRect(QRect(50, 60, 2 * static_cast<int>(percent), 20), color);
        }
    }

    painter.end();
    QWidget::paintEvent(event);
}

void RenderArea::resizeEvent(QResizeEvent* event)
{
    if (screen)
    {
        std::lock_guard guard(cameraM);
        delete screen;
        screen = new QImage(this->width(), this->height(), QImage::Format_ARGB32);
        double q = (double)this->width() / (double)screen->height();
        camera->updateShSw(q);
        config->sw = config->sh * q;
    }
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
            emit changedRenderMode(false);
            return;
        }
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
    if (renderInProcess)
        return;

    renderInProcess = true;
    rtRenderThread = new std::thread(&RenderArea::renderRayTracing, this);
    emit changedRenderMode(true);
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

void RenderArea::initCameraWithoutConfig()
{
    auto box = getBox(scene->figures);
    Point3D viewPoint = box.first + box.second;
    viewPoint /= 2;

    Point3D upVector = Point3D{0, 0, 1};
    Point3D origin = Point3D{box.first.x - (viewPoint.z - box.first.z) / M_SQRT3, viewPoint.y, viewPoint.z};

    double zf = (box.first.x - origin.x) / 2;
    double zb = (box.second.x - origin.x) + (box.second.x - box.first.x) / 2;
    double sh = M_SQRT3 * zf * 2.0;
    double sw = sh / screen->height() * screen->width();

    {
        std::lock_guard cguard(cameraM);
        *camera = Camera{origin, viewPoint, upVector, zf, zb, sw, sh};
    }
}
