#include <QApplication>
#include <QtWidgets>

#include "MainWindow.h"
#include "SceneKeeper.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    //read config, scene

    ConfigKeeper configKeeper;
    SceneKeeper sceneKeeper;

    auto scene = sceneKeeper.getSceneDescription("../data/example.scene");
    configKeeper.readConfig("../data/example.render");
    Camera camera{
        configKeeper.state.eye, configKeeper.state.view, configKeeper.state.up, configKeeper.state.zf,
        configKeeper.state.zb, configKeeper.state.sw, configKeeper.state.sh
    };

    MainWindow window{nullptr,&camera, &scene, &configKeeper.state};
    window.show();
    Q_INIT_RESOURCE(resources);
    return app.exec();
}
