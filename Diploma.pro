QT += core gui widgets opengl openglwidgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Scene.cpp \
    UIController.cpp \
    Cube.cpp \
    CubeModel.cpp

HEADERS += \
    MainWindow.hpp \
    Scene.hpp \
    UIController.hpp \
    Cube.hpp \
    CubeModel.hpp

RESOURCES += \
    shaders.qrc
