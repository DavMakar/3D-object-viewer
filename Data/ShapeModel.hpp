#ifndef SHAPE_MODEL_HPP
#define SHAPE_MODEL_HPP

#include <QAbstractListModel>
#include <QModelIndex>
#include "Shape.hpp"

class ShapeModel : public QAbstractListModel{
    Q_OBJECT
public:
    ShapeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index , int role) const override;
    const Shape& shapeAt(int index) const;
    void addShape(const Shape& shape); 
    const QList<Shape>& getShapes() const;

    void toggleShapeSelection(int index);

    bool isShapeSelected(int index) const;

    QList<Shape>::iterator begin();
    QList<Shape>::iterator end();
    
private:
    QList<Shape> m_ShapeList;
};

#endif //SHAPE_MODEL_HPP

