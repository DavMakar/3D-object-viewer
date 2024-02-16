#ifndef CUBE_LOADER_HPP
#define CUBE_LOADER_HPP

#include <QList>
#include "Cube.hpp"

class QJsonObject;

class CubeLoader{
public:
    static QList<Cube> loadCubesFromJson(const QString& filename); 
private:
    static QVector3D parseVectorFromJsonObj(const QJsonObject& obj, const QStringList& prefixes);
};

#endif //CUBE_LOADER_HPP
