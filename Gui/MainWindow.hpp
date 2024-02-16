#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include "UIController.hpp"
#include "Scene.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenu();

private slots:
    void onOpenAction();

private:
    QAction* openAction;

private:
    UIController *uiController;
    Scene *scene;
};

#endif // MAINWINDOW_H
