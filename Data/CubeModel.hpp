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
    void addCube(const Cube& cube);
    const Cube& cubeAt(int index) const;
private:
    QList<Cube> m_CubeList;
};

#endif //CUBE_MODEL_HPP
