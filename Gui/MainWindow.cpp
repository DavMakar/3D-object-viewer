#include "MainWindow.hpp"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QListView>
#include <QMenuBar>

#include "../Data/CubeLoader.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    openAction = new QAction(tr("&open"), this);
    openAction->setStatusTip(tr("Open objects file"));
    connect(openAction,&QAction::triggered, this, &MainWindow::onOpenAction);
    
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

    createMenu();
    //setLayout(layout);

    connect(uiController, &UIController::addCubeRequested, scene, &Scene::onAddCubeRequest);
}

void MainWindow::createMenu(){
    QMenu* menu;
    menu = menuBar()->addMenu(tr("&Load"));
    menu->addAction(openAction);
}

void MainWindow::onOpenAction(){
    QString filename = QFileDialog::getOpenFileName(this);
    if(filename.isEmpty()){
        qDebug() << "Empty file";
        return;
    }
    scene->loadCubes(CubeLoader::loadCubesFromJson(filename));
}

MainWindow::~MainWindow()
{
}
