//
// Created by anton on 6/19/25.
//

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <mutex>
#include <qprogressbar.h>
#include <qstackedlayout.h>
#include <QWidget>

#include "Camera.h"
#include "optics.h"
#include <thread>

#include "FilledRenderStrategy.h"
#include "WireframeRenderStrategy.h"

class RenderArea final : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget* parent, Camera* camera_, SceneDescription* scene_, ConfigState* config_);
    void saveRender(const QString& fileName) const;
    void saveConfig(const QString& fileName);
    void setConfig(const ConfigState &config);

signals:
    void requestUpdate();
    void changedRenderMode(bool newMode);

public slots:
    void onChangeRenderMode(const bool &mode);
    void onBackgroundColorChanged(const QColor &color);
    void onGammaChanged(const double &gamma);
    void onTracingDepthChanged(const int &depth);

protected slots:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private slots:
    void onRequestUpdate();

private:
    void rtRenderStart();
    void renderWireframes();
    void renderRayTracing();

private:
    QImage* screen;
    Camera* camera;
    SceneDescription* scene;
    ConfigState* config;
    FilledRenderStrategy filledRenderStrategy{};
    WireframeRenderStrategy wireframeRenderStrategy{};

    Point3D moveStep{};
    std::mutex stepM;
    std::mutex cameraM;

    std::thread* wireRenderThread{};
    bool wireRenderThreadRunning{};

    std::thread* rtRenderThread{};
    bool rtRenderThreadRunning{};
    QImage* render{};

    int progress{};
    QProgressBar* progressBar{};
    QStackedLayout* stackLayout{};

    bool isRenderShowing{};

    bool keyWPressed{};
    bool keyAPressed{};
    bool keySPressed{};
    bool keyDPressed{};
    bool keyCtrlPressed{};
    bool keyShiftPressed{};
    bool renderInProcess{};
};

inline void RenderArea::onRequestUpdate()
{
    update();
}


#endif //RENDERAREA_H
