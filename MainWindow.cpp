#include "MainWindow.hpp"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    uiController = new UIController;
    uiController->setMaximumWidth(100);
    scene = new Scene;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(uiController);
    layout->addWidget(scene);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    setLayout(layout);
    connect(uiController, &UIController::addCubeRequested, scene, &Scene::onAddCubeRequest);
}

MainWindow::~MainWindow()
{
}
