QT += core gui widgets opengl openglwidgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    Gui\MainWindow.cpp \
    Gui\Scene.cpp \
    Gui\UIController.cpp \
    Gui\ShapesListViewDelegate.cpp \
    Data\Shape.cpp \
    Data\ShapeModel.cpp \
    Data\ShapeSerializer.cpp \
    ShaderManager\ShaderManager.cpp \
    Renderer\Renderer.cpp \
    Camera\Camera.cpp \
    MousePicker\MousePicker.cpp

HEADERS += \
    Gui\MainWindow.hpp \
    Gui\Scene.hpp \
    Gui\UIController.hpp \
    Gui\ShapesListViewDelegate.hpp \
    Data\Shape.hpp \
    Data\ShapeModel.hpp \
    Data\ShapeSerializer.hpp \
    ShaderManager\ShaderManager.hpp \
    Renderer\Renderer.hpp \
    Camera\Camera.hpp \
    MousePicker\MousePicker.hpp

RESOURCES += \
    icon.qrc \
    shaders.qrc

DISTFILES +=
