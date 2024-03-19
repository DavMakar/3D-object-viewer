#include "ShapeSerializer.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QList<Shape> ShapeSerializer::importShapesFromJson(const QString &filename)
{
    QList<Shape> shapes;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        return shapes;
    };
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());

    if (!jsonDoc.isArray())
    {
        qDebug() << "Invalid JSON format. Expected array.";
        return shapes;
    }

    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value : jsonArray)
    {
        if (!value.isObject())
        {
            qDebug() << "Invalid JSON format. Expected object.";
            continue;
        }
        
        QJsonObject jsonObj = value.toObject();
        
        QString type = jsonObj["type"].toString();
        QJsonObject positionObj = jsonObj["position"].toObject();
        QJsonObject colorObj = jsonObj["color"].toObject();

        auto posVec = parseVectorFromJsonObj(positionObj, {"x", "y", "z"});
        auto colorVec = parseVectorFromJsonObj(colorObj, {"r", "g", "b"});

        shapes.append({type,posVec, colorVec});
    }

    file.close();
    return shapes;
}

bool ShapeSerializer::exportShapesToJson(const QList<Shape>& shapes, const QString & filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return false;
    }

    QJsonArray jsonArray;
    for (const auto& shape : shapes)
    {
        QJsonObject shapeObj;

        shapeObj["type"] = shape.type();

        QJsonObject positionObj;
        auto shapePos = shape.getPosition();
        positionObj["x"] = shapePos.x();
        positionObj["y"] = shapePos.y();
        positionObj["z"] = shapePos.z();
        shapeObj["position"] = positionObj;

        QJsonObject colorObj;
        auto shapeColor = shape.getColor();
        colorObj["r"] = shapeColor.x();
        colorObj["g"] = shapeColor.y();
        colorObj["b"] = shapeColor.z();
        shapeObj["color"] = colorObj;

        jsonArray.append(shapeObj);
    }
    QJsonDocument jsonDoc(jsonArray);
    file.write(jsonDoc.toJson());
    file.close();
    return true;
}

QVector3D ShapeSerializer::parseVectorFromJsonObj(const QJsonObject &obj, const QStringList &prefixes)
{
    float value1 = static_cast<float>(obj[prefixes[0]].toDouble());
    float value2 = static_cast<float>(obj[prefixes[1]].toDouble());
    float value3 = static_cast<float>(obj[prefixes[2]].toDouble());

    return QVector3D(value1, value2, value3);
}
