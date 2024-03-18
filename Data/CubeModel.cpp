#include "CubeModel.hpp"

CubeModel::CubeModel(QObject *parent)
    :QAbstractListModel(parent)
{
}

int CubeModel::rowCount(const QModelIndex &parent) const
{
    return m_CubeList.size();
}

QVariant CubeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    
    if(index.row() >= m_CubeList.size()){
        return QVariant();
    }
    if(role == Qt::DisplayRole){
        return QString("Object %1").arg(index.row()+1);
    }
    return QVariant();
}

void CubeModel::addCube(const Cube &cube){
    int row = m_CubeList.size();
    beginInsertRows(QModelIndex(),row,row);
    m_CubeList.prepend(cube);
    endInsertRows();
}
const QList<Cube> &CubeModel::getCubes() const
{
    return m_CubeList;
}
const Cube &CubeModel::cubeAt(int index) const
{
    return m_CubeList.at(index);
}

QList<Cube>::iterator CubeModel::begin()
{
    return m_CubeList.begin();
}

QList<Cube>::iterator CubeModel::end(){
    return m_CubeList.end();
}

void CubeModel::toggleCubeSelection(int index){
    if (index >= 0 && index < m_CubeList.size()) {
        Cube& cube = m_CubeList[index];
        cube.setSelected(!cube.isSelected());
        // Notify any views that the data has changed
        QModelIndex modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex, modelIndex);
    }
}
bool CubeModel::isCubeSelected(int index) const
{
    if (index >= 0 && index < m_CubeList.size()) {
        const Cube& cube = m_CubeList[index];
        return cube.isSelected();
    }
    return false;
};