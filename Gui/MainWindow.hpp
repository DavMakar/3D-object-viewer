#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QListView;
class UIController;
class Scene;
class QMenuBar;
class QItemSelectionModel;
class QItemSelection;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QMenuBar* createMenuBar();
    QToolBar* createToolBar();
    
private slots:
    void onImportAction();
    void onExportAction();
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void onDeleteButtonClicked(const QModelIndex& index);
    void onToggleMoveMode(bool checked);
    void onToggleSelectMode(bool checked);

private:
    QAction* importAction;
    QAction* exportAction;
    QAction* clearAction;
    
    QAction* moveModeAction;
    QAction* selectModeAction;

    QItemSelectionModel* selectionModel;
    QListView* shapesListView;
    UIController *uiController;
    Scene *scene;
};

#endif // MAINWINDOW_H
