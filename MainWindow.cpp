#include "MainWindow.hpp"
#include <QHBoxLayout>
#include <QListView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    uiController = new UIController;
    uiController->setMaximumWidth(100);
    scene = new Scene;
    QListView* cubeView = new QListView;
    cubeView->setMaximumWidth(100);
    cubeView->setModel(&scene->getModel());

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(uiController);
    layout->addWidget(scene);
    layout->addWidget(cubeView);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    setLayout(layout);
    connect(uiController, &UIController::addCubeRequested, scene, &Scene::onAddCubeRequest);
}

MainWindow::~MainWindow()
{
}
