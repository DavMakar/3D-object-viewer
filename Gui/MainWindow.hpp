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
    void onExportAction();
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void onDeleteButtonClicked(const QModelIndex& index);
private:
    QAction* importAction;
    QAction* exportAction;
    QAction* clearAction;

    QItemSelectionModel* selectionModel;
    QListView* shapesListView;
    UIController *uiController;
    Scene *scene;
};

#endif // MAINWINDOW_H
