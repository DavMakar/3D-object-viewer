#include "ShapeModel.hpp"

ShapeModel::ShapeModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ShapeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_ShapeList.size();
}

QVariant ShapeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_ShapeList.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto& shape = shapeAt(index.row());
        return QString("%1 %2").arg(shape.type()).arg(index.row());
    }
    return QVariant();
}

const Shape& ShapeModel::shapeAt(int index) const
{
    return m_ShapeList.at(index);
}

void ShapeModel::addShape(const Shape& shape)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_ShapeList.append(shape);
    endInsertRows();
}

void ShapeModel::removeShape(int index)
{
    m_ShapeList.removeAt(index);
}

const QList<Shape>& ShapeModel::getShapes() const
{
    return m_ShapeList;
}

void ShapeModel::toggleShapeSelection(int index)
{
    if (index < 0 || index >= m_ShapeList.size())
        return;

    Shape& shape = m_ShapeList[index];
    shape.setSelected(!shape.isSelected());
    QModelIndex idx = createIndex(index,0);
        emit dataChanged(idx, idx);
}

bool ShapeModel::isShapeSelected(int index) const
{
    if (index < 0 || index >= m_ShapeList.size())
        return false;

    const Shape& shape = m_ShapeList.at(index);
    return shape.isSelected();
}

QList<Shape>::iterator ShapeModel::begin()
{
    return m_ShapeList.begin();
}

QList<Shape>::iterator ShapeModel::end()
{
    return m_ShapeList.end();
}
