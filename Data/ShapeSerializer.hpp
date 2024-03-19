#ifndef SHAPE_SERIALIZER_HPP
#define SHAPE_SERIALIZER_HPP

#include <QList>
#include "Shape.hpp"

class QJsonObject;

class ShapeSerializer {
public:
    static QList<Shape> importShapesFromJson(const QString& filename);
    static bool exportShapesToJson(const QList<Shape>& shapeList, const QString& filename);
private:
    static QVector3D parseVectorFromJsonObj(const QJsonObject& obj, const QStringList& prefixes);
};

#endif //SHAPE_SERIALIZER_HPP
