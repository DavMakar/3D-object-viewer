#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>

class UIController : public QWidget
{
    Q_OBJECT

public:
    explicit UIController(QWidget *parent = nullptr);
    ~UIController();

signals:
    void addCubeRequested(float posX, float posY, float posZ, const QVector3D &color);

private slots:
    void onAddCubeButtonClicked();

private:
    QLineEdit *posXLineEdit;
    QLineEdit *posYLineEdit;
    QLineEdit *posZLineEdit;
    QComboBox *colorComboBox;
    QPushButton *addCuboidButton;
};

#endif // UICONTROLLER_H

