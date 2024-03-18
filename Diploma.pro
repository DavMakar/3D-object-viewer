QT += core gui widgets opengl openglwidgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    Gui\MainWindow.cpp \
    Gui\Scene.cpp \
    Gui\UIController.cpp \
    Data\Cube.cpp \
    Data\CubeModel.cpp \
    Data\CubeSerializer.cpp \
    ShaderManager\ShaderManager.cpp \
    Renderer\Renderer.cpp \
    Camera\Camera.cpp \
    MousePicker\MousePicker.cpp

HEADERS += \
    Gui\MainWindow.hpp \
    Gui\Scene.hpp \
    Gui\UIController.hpp \
    Data\Cube.hpp \
    Data\CubeModel.hpp \
    Data\CubeSerializer.hpp \
    ShaderManager\ShaderManager.hpp \
    Renderer\Renderer.hpp \
    Camera\Camera.hpp \
    MousePicker\MousePicker.hpp

RESOURCES += \
    shaders.qrc

DISTFILES +=
