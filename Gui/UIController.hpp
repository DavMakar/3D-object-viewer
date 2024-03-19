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
signals:
    void addShapeRequested(const QString& type,const QVector3D& posVec, const QVector3D &color);

private slots:
    void onAddShapeButtonClicked();
    void onLineEditClick();

private:
    QComboBox *shapeComboBox;
    QLineEdit *posXLineEdit;
    QLineEdit *posYLineEdit;
    QLineEdit *posZLineEdit;
    QComboBox *colorComboBox;
    QPushButton *addShapeButton;
};

#endif // UICONTROLLER_H

