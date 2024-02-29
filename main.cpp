#include <QSurfaceFormat>
#include <QApplication>

//#define SCENE

#ifndef SCENE 
    #include "Gui/MainWindow.hpp"
#else
    #include "Gui/Scene.hpp"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

#ifndef SCENE 
    MainWindow mw;
    mw.show();   
#else
    Scene s;
    s.show();
#endif

    return a.exec();
}
