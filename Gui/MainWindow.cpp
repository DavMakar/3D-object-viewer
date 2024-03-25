#include "MainWindow.hpp"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QListView>
#include <QMenuBar>
#include <QEvent>
#include <QAction>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QToolBar>

#include "../Data/ShapeSerializer.hpp"
#include "ShapesListViewDelegate.hpp"
#include "UIController.hpp"
#include "Scene.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/Icon/logo1"));
    importAction = new QAction(tr("&Import objects"), this);
    importAction->setStatusTip(tr("load objects from json"));
    exportAction = new QAction(tr("&Export objects"),this);
    exportAction->setStatusTip(tr("export objects from json"));
    clearAction = new QAction(tr("&Clear Scene"), this);
    clearAction->setStatusTip(tr("remove all objects from scene"));
 
    setMenuBar(createMenuBar());
    addToolBar(createToolBar());

    uiController = new UIController;
    uiController->setMaximumWidth(100);

    scene = new Scene;

    shapesListView = new QListView;
    shapesListView->setMaximumWidth(100);
    shapesListView->setModel(&scene->getModel());
    shapesListView->setMouseTracking(true);
    ShapesListViewDelegate* delegate = new ShapesListViewDelegate(this);
    shapesListView->setItemDelegate(delegate);

    selectionModel = shapesListView->selectionModel();

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    layout->addWidget(uiController);
    layout->addWidget(scene);
    layout->addWidget(shapesListView);
    setCentralWidget(centralWidget);

    connect(importAction,&QAction::triggered, this, &MainWindow::onImportAction);
    connect(exportAction,&QAction::triggered, this, &MainWindow::onExportAction);
    connect(clearAction, &QAction::triggered, scene, &Scene::onClearSceneAction);

    connect(uiController, &UIController::addShapeRequested, scene, &Scene::onAddShapeRequest);
    connect(selectionModel, &QItemSelectionModel::selectionChanged, this , &MainWindow::onSelectionChanged);
    connect(delegate, &ShapesListViewDelegate::deleteButtonClicked, this, &MainWindow::onDeleteButtonClicked);
}

QMenuBar* MainWindow::createMenuBar(){
    QMenuBar* menuBar = new QMenuBar();
    QMenu* importMenu = new QMenu(tr("&Import"));
    QMenu* exportMenu = new QMenu(tr("&Export"));
    QMenu* sceneMenu = new QMenu(tr("&Scene"));

    menuBar->addMenu(importMenu);
    menuBar->addMenu(exportMenu);
    menuBar->addMenu(sceneMenu);
    importMenu->addAction(importAction);
    exportMenu->addAction(exportAction);
    sceneMenu->addAction(clearAction);
    return menuBar;
}

QToolBar* MainWindow::createToolBar(){
    QToolBar* toolBar = addToolBar(tr("Modes"));

    moveModeAction = new QAction(QIcon(":/Icon/move.png"),tr("Move"), this);
    moveModeAction->setCheckable(true);
    moveModeAction->setChecked(true);
    toolBar->addAction(moveModeAction);

    selectModeAction = new QAction(QIcon(":/Icon/selection.png"),tr("Select"), this);
    selectModeAction->setCheckable(true);
    toolBar->addAction(selectModeAction);

    connect(moveModeAction, &QAction::triggered, this, &MainWindow::onToggleMoveMode);    
    connect(selectModeAction, &QAction::triggered, this, &MainWindow::onToggleSelectMode);

    return toolBar;
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

void MainWindow::onDeleteButtonClicked(const QModelIndex& index)
{
    scene->getModel().removeShape(index.row());
    shapesListView->update();
    shapesListView->selectionModel()->clearSelection();
}

void MainWindow::onToggleMoveMode(bool checked)
{
    if (checked) {
        if (selectModeAction->isChecked()) {
            selectModeAction->setChecked(false);
        }
        scene->setMode(Scene::Mode::MOVE);
    }
}

void MainWindow::onToggleSelectMode(bool checked)
{
    if (checked) {
        if (moveModeAction->isChecked()) {
            moveModeAction->setChecked(false);
        }
        scene->setMode(Scene::Mode::SELECT);
    }
}

MainWindow::~MainWindow() {}
