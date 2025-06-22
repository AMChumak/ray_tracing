//
// Created by anton on 6/19/25.
//

#include "MainWindow.h"

#include <iostream>
#include <qcolordialog.h>
#include <qevent.h>
#include <qfiledialog.h>

#include "AboutWindow.h"
#include "SceneKeeper.h"

MainWindow::MainWindow(QWidget* parent, Camera* camera_, SceneDescription* scene_, ConfigState* config_):
    QMainWindow(parent), m_layout(nullptr), m_window(nullptr)
{
    //prepare menu

    m_loadButton = new QPushButton("Load scene");
    m_loadRender = new QPushButton("Load Config");
    m_saveConfig = new QPushButton("Save Config");
    m_saveRender = new QPushButton("Export");
    m_init = new QPushButton("Init");
    m_about = new QPushButton("About");
    m_renderMode = new QCheckBox("Render Mode");
    m_renderMode->setStyleSheet(R"(
        QCheckBox::indicator {
            width: 50px;
            height: 25px;
        }
        QCheckBox::indicator:checked {
            background-color: green;
        }
    )");

    line = new QFrame;
    line->setMinimumWidth(176);
    line->setFrameShape(QFrame::HLine);  // Горизонтальная линия
    line->setFrameShadow(QFrame::Sunken); // Стиль линии (вдавленная)

    m_renderSettings = new QLabel("Render Settings");
    m_renderSettings->setFont(QFont("Roboto", 14));
    m_renderSettings->setAlignment(Qt::AlignCenter);
    m_renderSettings->setMinimumWidth(176);
    m_renderSettings->setMinimumHeight(40);

    m_backgroundLabel = new QLabel("Background:");
    m_backgroundLabel->setFont(QFont("Roboto", 10));
    m_backgroundLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_paletteButton = new QPushButton("choose color");
    m_paletteButton->setAutoFillBackground(true);
    QPalette palette = m_paletteButton->palette();
    palette.setColor(QPalette::Button, QColor(config_->br,config_->bg,config_->bb));
    m_paletteButton->setPalette(palette);
    m_paletteButton->setFont(QFont("Roboto", 10));

    m_gammaLabel = new QLabel("Gamma:");
    m_gammaLabel->setFont(QFont("Roboto", 10));
    m_gammaLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_gammaSpinBox = new QDoubleSpinBox();
    m_gammaSpinBox->setDecimals(1);
    m_gammaSpinBox->setValue(config_->gamma);
    m_gammaSpinBox->setMinimum(0.1);
    m_gammaSpinBox->setMaximum(10.0);
    m_gammaSpinBox->setFont(QFont("Roboto", 10));

    m_depthLabel = new QLabel("Tracing Depth:");
    m_depthLabel->setFont(QFont("Roboto", 10));
    m_depthLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_depthSpinBox = new QSpinBox();
    m_depthSpinBox->setValue(config_->depth);
    m_depthSpinBox->setMinimum(1);
    m_depthSpinBox->setMaximum(10);

    m_scrollLayout = new QVBoxLayout();
    m_scrollLayout->setSpacing(5);
    m_scrollLayout->setContentsMargins(10, 20, 10, 10);
    m_scrollLayout->addWidget(m_loadButton);
    m_scrollLayout->addWidget(m_loadRender);
    m_scrollLayout->addWidget(m_saveConfig);
    m_scrollLayout->addWidget(m_saveRender);
    m_scrollLayout->addWidget(m_init);
    m_scrollLayout->addWidget(m_about);
    m_scrollLayout->addWidget(m_renderMode);
    m_scrollLayout->addWidget(line);
    m_scrollLayout->addWidget(m_renderSettings);
    m_scrollLayout->addWidget(m_backgroundLabel);
    m_scrollLayout->addWidget(m_paletteButton);
    m_scrollLayout->addWidget(m_gammaLabel);
    m_scrollLayout->addWidget(m_gammaSpinBox);
    m_scrollLayout->addWidget(m_depthLabel);
    m_scrollLayout->addWidget(m_depthSpinBox);




    m_scrollWidget = new QWidget();
    m_scrollWidget->setLayout(m_scrollLayout);
    m_scrollWidget->setMaximumWidth(200);
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidget(m_scrollWidget);
    m_scrollArea->setMaximumWidth(200);
    //set view
    m_renderArea = new RenderArea(this, camera_, scene_, config_);
    m_renderArea->resize(QSize(this->size().width() - 200, this->size().height()));
    m_layout = new QHBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_renderArea);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_scrollArea);


    m_window = new QWidget();
    m_window->setLayout(m_layout);
    setCentralWidget(m_window);


    //connections
    connect(m_renderMode, &QCheckBox::toggled, m_renderArea, &RenderArea::onChangeRenderMode);
    connect(m_renderArea, &RenderArea::changedRenderMode, this, &MainWindow::onRenderModeChanged);
    connect(m_paletteButton, &QPushButton::clicked, this, &MainWindow::onColorChoose);
    connect(m_gammaSpinBox, &QDoubleSpinBox::valueChanged, m_renderArea, &RenderArea::onGammaChanged);
    connect(m_depthSpinBox, &QSpinBox::valueChanged, m_renderArea, &RenderArea::onTracingDepthChanged);
    connect(m_saveRender, &QPushButton::clicked, this, &MainWindow::onExport);
    connect(m_saveConfig, &QPushButton::clicked, this, &MainWindow::onSaveConfig);
    connect(m_loadRender, &QPushButton::clicked, this, &MainWindow::onLoadConfig);
    connect(m_about, &QPushButton::clicked, this, &MainWindow::onAbout);
    connect(m_init, &QPushButton::clicked, m_renderArea, &RenderArea::initCameraPosition);
    connect(m_loadButton, &QPushButton::clicked, this, &MainWindow::onLoadScene);

    setWindowTitle("AMC Ray Tracing");
    setMinimumSize(840, 480);
    setFocusProxy(m_renderArea);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    m_renderArea->resize(QSize(this->size().width() - 200, this->size().height()));
    QMainWindow::resizeEvent(event);
}

void MainWindow::onRenderModeChanged(const bool mode) const
{
    if (mode && !m_renderMode->isChecked())
        m_renderMode->setCheckState(Qt::CheckState::Checked);
    else if (!mode && m_renderMode->isChecked())
        m_renderMode->setCheckState(Qt::CheckState::Unchecked);
}

void MainWindow::onColorChoose() const
{
    QColor color = QColorDialog::getColor();
    QPalette palette = m_paletteButton->palette();
    palette.setColor(QPalette::Button, color);
    m_paletteButton->setPalette(palette);
    m_renderArea->onBackgroundColorChanged(color);
}

void MainWindow::onExport()
{
    const QString fileName = QFileDialog::getSaveFileName(this, "Export your beautiful render", QDir::homePath(), "*.png");
    if (fileName.isEmpty()) {
        return;
    }
    m_renderArea->saveRender(fileName);
}

void MainWindow::onAbout()
{
    m_aboutWindow = new AboutWindow(this);
    m_aboutWindow->show();
}

void MainWindow::onSaveConfig()
{
    const QString fileName = QFileDialog::getSaveFileName(this, "Save configuration", QDir::homePath(), "*.render");
    if (fileName.isEmpty()) {
        return;
    }
    m_renderArea->saveConfig(fileName);
}

void MainWindow::onLoadConfig()
{
    const QString fileName = QFileDialog::getOpenFileName(this, "Choose configuration", QDir::homePath(), "*.render");
    if (fileName.isEmpty()) {
        return;
    }
    ConfigKeeper keeper;
    keeper.readConfig(fileName.toStdString());
    m_depthSpinBox->setValue(keeper.state.depth);
    m_gammaSpinBox->setValue(keeper.state.gamma);
    QPalette palette = m_paletteButton->palette();
    palette.setColor(QPalette::Button, QColor(keeper.state.br,keeper.state.bg,keeper.state.bb));
    m_paletteButton->setPalette(palette);
    m_renderArea->setConfig(keeper.state);
}

void MainWindow::onLoadScene()
{
    const QString sceneFileName = QFileDialog::getOpenFileName(this, "Choose scene", QDir::homePath(), "*.scene");
    if (sceneFileName.isEmpty()) {
        return;
    }
    SceneKeeper keeper;
    SceneDescription scene = keeper.getSceneDescription(sceneFileName.toStdString());

    QString configName = sceneFileName.sliced(0,sceneFileName.lastIndexOf('.') + 1);
    configName = configName + "render";
    ConfigKeeper keeper2;
    keeper2.readConfig(configName.toStdString());
    m_renderArea->loadScene(scene, keeper2.state, keeper2.configLoaded);
}
