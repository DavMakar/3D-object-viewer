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
        return QString("Cube %1").arg(index.row()+1);
    }
    return QVariant();
}

void CubeModel::addCube(const Cube &cube){
    int row = m_CubeList.size();
    beginInsertRows(QModelIndex(),row,row);
    m_CubeList.prepend(cube);
    endInsertRows();
}
const Cube& CubeModel::cubeAt(int index) const
{
    return m_CubeList.at(index);
}