#ifndef CUBE_MODEL_HPP
#define CUBE_MODEL_HPP

#include <QAbstractListModel>
#include <QModelIndex>
#include "Cube.hpp"

class CubeModel : public QAbstractListModel{
    Q_OBJECT
public:
    CubeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index , int role) const;
    const Cube& cubeAt(int index) const;
    void addCube(const Cube& cube);

    void toggleCubeSelection(int index);

    bool isCubeSelected(int index) const;

    QList<Cube>::iterator begin();
    QList<Cube>::iterator end();
    
private:
    QList<Cube> m_CubeList;
};

#endif //CUBE_MODEL_HPP
