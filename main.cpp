#include <QSurfaceFormat>
#include <QApplication>
#include "Gui/MainWindow.hpp"
//#include "Gui/Scene.hpp"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

//    Scene s;
//    s.show();
    MainWindow mw;
    mw.show();
    return a.exec();
}
