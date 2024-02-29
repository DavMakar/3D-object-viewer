#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QAction;
class QListView;
class UIController;
class Scene;
class QMenuBar;
class QItemSelectionModel;
class QItemSelection;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QMenuBar* createMenuBar();

private slots:
    void onImportAction();
    void onExportAction(); //TODO
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    QAction* importAction;
    QAction* exportAction;

    QItemSelectionModel* selectionModel;
    QListView* cubeView;
    UIController *uiController;
    Scene *scene;
};

#endif // MAINWINDOW_H
