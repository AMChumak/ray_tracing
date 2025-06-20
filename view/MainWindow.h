#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if defined MAKE_UI_LIB
#define UI_LIB_EXPORT Q_DECL_EXPORT
#else
#define UI_LIB_EXPORT Q_DECL_IMPORT
#endif
#include <qmainwindow.h>
#include <QLayout>

#include "RenderArea.h"

class UI_LIB_EXPORT MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent, Camera* camera_, SceneDescription* scene_, ConfigState* config_);


private:
    QVBoxLayout* m_layout;
    QWidget* m_window;
    RenderArea* m_renderArea;
};


#endif //MAINWINDOW_H
