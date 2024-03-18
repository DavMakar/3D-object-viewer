#ifndef CUBE_SERIALIZER_HPP
#define CUBE_SERIALIZER_HPP

#include <QList>
#include "Cube.hpp"

class QJsonObject;

class CubeSerializer{
public:
    static QList<Cube> importCubesFromJson(const QString& filename);
    static bool exportCubesToJson(const QList<Cube>& cubeList, const QString& filename);
private:
    static QVector3D parseVectorFromJsonObj(const QJsonObject& obj, const QStringList& prefixes);
};

#endif //CUBE_SERIALIZER_HPP
