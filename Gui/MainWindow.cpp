#include "MainWindow.hpp"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QListView>
#include <QMenuBar>
#include <QEvent>
#include <QAction>
#include <QItemSelectionModel>
#include <QItemSelection>

#include "../Data/ShapeSerializer.hpp"
#include "UIController.hpp"
#include "Scene.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    importAction = new QAction(tr("&Import objects"), this);
    importAction->setStatusTip(tr("load objects from json"));
    exportAction = new QAction(tr("&Export objects"),this);
    exportAction->setStatusTip(tr("export objects from json"));

    connect(importAction,&QAction::triggered, this, &MainWindow::onImportAction);
    connect(exportAction,&QAction::triggered, this, &MainWindow::onExportAction);

    uiController = new UIController;
    uiController->setMaximumWidth(100);

    scene = new Scene;

    cubeView = new QListView;
    cubeView->setMaximumWidth(100);
    cubeView->setModel(&scene->getModel());

    selectionModel = cubeView->selectionModel();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMenuBar(createMenuBar());
    layout->addWidget(uiController);
    layout->addWidget(scene);
    layout->addWidget(cubeView);

    this->setLayout(layout);

    connect(uiController, &UIController::addShapeRequested, scene, &Scene::onAddShapeRequest);
    connect(selectionModel, &QItemSelectionModel::selectionChanged, this , &MainWindow::onSelectionChanged);
}

QMenuBar* MainWindow::createMenuBar(){
    QMenuBar* menuBar = new QMenuBar();
    QMenu* importMenu = new QMenu(tr("&Import"));
    QMenu* exportMenu = new QMenu(tr("&Export"));
    menuBar->addMenu(importMenu);
    menuBar->addMenu(exportMenu);
    importMenu->addAction(importAction);
    exportMenu->addAction(exportAction);
    return menuBar;
}

void MainWindow::onImportAction(){
    QString filename = QFileDialog::getOpenFileName(this);
    if(filename.isEmpty()){
        qDebug() << "Empty file";
        return;
    }
    scene->loadShapes(ShapeSerializer::importShapesFromJson(filename));
}

void MainWindow::onExportAction(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Export Cubes"), "", tr("JSON Files (*.json)"));
    if (filename.isEmpty()) {
        qDebug() << "Empty file";
        return;
    }
    if (!ShapeSerializer::exportShapesToJson(scene->getModel().getShapes(), filename)) {
        qDebug() << "Export failed";
        return;
    }
    qDebug() << "Export successful";
}

void MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QModelIndex selectedIndex = selected.indexes().isEmpty() ? QModelIndex() : selected.indexes().first();
    QModelIndex deselectedIndex = deselected.indexes().isEmpty() ? QModelIndex() : deselected.indexes().first();

    if (selectedIndex.isValid()) {
        scene->getModel().toggleShapeSelection(selectedIndex.row());
    }

    if (deselectedIndex.isValid()) {
        scene->getModel().toggleShapeSelection(deselectedIndex.row());
    }
}

MainWindow::~MainWindow() {}

//bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
//    if (event->type() == QEvent::MouseButtonPress) {
//        qDebug() << "Mouse pressed!";
//        currentWidget = dynamic_cast<QWidget*>(obj);
//        installEventFilterOnWidgets(currentWidget);
//    }
//    return QWidget::eventFilter(obj, event);
//}

//void MainWindow::installEventFilterOnWidgets(QWidget* widget) {
//    uninstallEventFilterOnWidgets(this);
//    if (widget)
//        widget->installEventFilter(this);
//}

//void MainWindow::uninstallEventFilterOnWidgets(QWidget* widget) {
//    if (!widget)
//        return;
//    const QObjectList& children = widget->children();
//    for (QObject* child : children) {
//        if (QWidget* childWidget = qobject_cast<QWidget*>(child)) {
//            childWidget->removeEventFilter(this);
//            uninstallEventFilterOnWidgets(childWidget);
//        }
//    }
//}
