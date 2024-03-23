#include "UIController.hpp"
#include <QColorDialog>
#include <QVector3D>

UIController::UIController(QWidget *parent)
    : QWidget(parent)
{    
    shapeComboBox = new QComboBox(this);
    shapeComboBox->addItems({"Cube","Pyramid","Sphere"});

    posXLineEdit = new QLineEdit(this);
    posXLineEdit->setPlaceholderText("Enter posX");

    posYLineEdit = new QLineEdit(this);
    posYLineEdit->setPlaceholderText("Enter posY");

    posZLineEdit = new QLineEdit(this);
    posZLineEdit->setPlaceholderText("Enter posZ");

    colorComboBox = new QComboBox(this);
    colorComboBox->addItems({"Red", "Green", "Blue"});
    
    addShapeButton = new QPushButton("Add Object", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(shapeComboBox);
    layout->addWidget(posXLineEdit);
    layout->addWidget(posYLineEdit);
    layout->addWidget(posZLineEdit);
    layout->addWidget(colorComboBox);
    layout->addWidget(addShapeButton);

    connect(addShapeButton, &QPushButton::clicked, this, &UIController::onAddShapeButtonClicked);
}

void UIController::onLineEditClick(){
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender());
    if (lineEdit)
        lineEdit->setReadOnly(false);
}

void UIController::onAddShapeButtonClicked()
{
    QString type = shapeComboBox->currentText();
    
    bool ok;
    float x = posXLineEdit->text().toFloat(&ok);
    posXLineEdit->clear();
    if (!ok) return;

    float y = posYLineEdit->text().toFloat(&ok);
    posYLineEdit->clear();
    if (!ok) return;

    float z = posZLineEdit->text().toFloat(&ok);
    posZLineEdit->clear();
    if (!ok) return;

    QString colorString = colorComboBox->currentText();

    QVector3D color;
    if (colorString == "Red"){
        color = QVector3D(1.0f, 0.0f, 0.0f);
    }else if (colorString == "Green"){
        color = QVector3D(0.0f, 1.0f, 0.0f);  
    }
    else {
        color = QVector3D(0.0f, 0.0f, 1.0f);  // BLue
    }
    emit addShapeRequested(type,QVector3D{x,y,z},color);
}
