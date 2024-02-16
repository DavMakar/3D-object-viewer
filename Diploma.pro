QT += core gui widgets opengl openglwidgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    Gui\MainWindow.cpp \
    Gui\Scene.cpp \
    Gui\UIController.cpp \
    Data\Cube.cpp \
    Data\CubeModel.cpp \
    Data\CubeLoader.cpp 

HEADERS += \
    Gui\MainWindow.hpp \
    Gui\Scene.hpp \
    Gui\UIController.hpp \
    Data\Cube.hpp \
    Data\CubeModel.hpp \
    Data\CubeLoader.hpp 

RESOURCES += \
    shaders.qrc

DISTFILES +=
