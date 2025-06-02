#include <QApplication>
#include <QtWidgets>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window{};
    window.show();
    //Q_INIT_RESOURCE(resources);
    return app.exec();
}