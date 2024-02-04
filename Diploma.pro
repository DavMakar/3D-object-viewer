QT += core gui widgets opengl openglwidgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Scene.cpp \
    UIController.cpp \
    Cube.cpp

HEADERS += \
    MainWindow.hpp \
    Scene.hpp \
    UIController.hpp \
    Cube.hpp

RESOURCES += \
    shaders.qrc
