#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined MAKE_UI_LIB
#define UI_LIB_EXPORT Q_DECL_EXPORT
#else
#define UI_LIB_EXPORT Q_DECL_IMPORT
#endif
#include <QCheckBox>
#include <QLabel>
#include <QMainWindow>
#include <QLayout>
#include <QScrollArea>
#include <QPushButton>
#include <qspinbox.h>


#include "AboutWindow.h"
#include "RenderArea.h"

class UI_LIB_EXPORT MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent, Camera* camera_, SceneDescription* scene_, ConfigState* config_);

protected slots:
    void resizeEvent(QResizeEvent* event) override;

public slots:
    void onRenderModeChanged(bool mode) const;
    void onColorChoose() const;
    void onExport();
    void onAbout();
    void onSaveConfig();
    void onLoadConfig();

private:
    QWidget* m_window;
    RenderArea* m_renderArea;
    QHBoxLayout* m_layout;
    QScrollArea* m_scrollArea;
    QWidget* m_scrollWidget;
    QVBoxLayout* m_scrollLayout;
    QPushButton* m_loadButton;
    QPushButton* m_loadRender;
    QPushButton* m_saveRender;
    QPushButton* m_saveConfig;
    QPushButton* m_about;
    AboutWindow* m_aboutWindow;
    QCheckBox* m_renderMode;
    QLabel* m_renderSettings;
    QLabel* m_depthLabel;
    QSpinBox* m_depthSpinBox;
    QLabel* m_gammaLabel;
    QDoubleSpinBox* m_gammaSpinBox;
    QLabel* m_backgroundLabel;
    QPalette* m_backgroundPalette;
    QPushButton* m_paletteButton;
    QFrame* line;
};


#endif //MAINWINDOW_H
