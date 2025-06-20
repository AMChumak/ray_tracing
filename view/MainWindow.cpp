//
// Created by anton on 6/19/25.
//

#include "MainWindow.h"

#include <iostream>
#include <qevent.h>

MainWindow::MainWindow(QWidget* parent, Camera* camera_, SceneDescription* scene_, ConfigState* config_):
    QMainWindow(parent), m_layout(nullptr), m_window(nullptr)
{
    m_renderArea = new RenderArea(this, camera_, scene_, config_);
    m_renderArea->resize(this->size());
    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_renderArea);

    m_window = new QWidget();
    m_window->setLayout(m_layout);
    setCentralWidget(m_window);

    setWindowTitle("AMC Ray Tracing");
    setMinimumSize(640, 480);
    setFocusProxy(m_renderArea);
}

