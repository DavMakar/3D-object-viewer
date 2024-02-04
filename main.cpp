#include "MainWindow.hpp"
#include <QSurfaceFormat>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow mw;
    mw.show();
    return a.exec();
}
