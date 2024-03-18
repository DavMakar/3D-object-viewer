#include "CubeSerializer.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QList<Cube> CubeSerializer::importCubesFromJson(const QString &filename)
{
    QList<Cube> cubeList;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        return cubeList;
    };
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());

    if (!jsonDoc.isArray())
    {
        qDebug() << "Invalid JSON format. Expected array.";
        return cubeList;
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
        QJsonObject positionObj = jsonObj["position"].toObject();
        QJsonObject colorObj = jsonObj["color"].toObject();

        auto posVec = parseVectorFromJsonObj(positionObj, {"x", "y", "z"});
        auto colorVec = parseVectorFromJsonObj(colorObj, {"r", "g", "b"});

        cubeList.append({posVec, colorVec});
    }

    file.close();
    return cubeList;
}

bool CubeSerializer::exportCubesToJson(const QList<Cube>& cubeList, const QString & filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return false;
    }

    QJsonArray jsonArray;
    for (const Cube& cube : cubeList)
    {
        QJsonObject cubeObj;

        QJsonObject positionObj;
        auto cubePos = cube.getPosition();
        positionObj["x"] = cubePos.x();
        positionObj["y"] = cubePos.y();
        positionObj["z"] = cubePos.z();
        cubeObj["position"] = positionObj;

        QJsonObject colorObj;
        auto cubeColor = cube.getColor();
        colorObj["r"] = cubeColor.x();
        colorObj["g"] = cubeColor.y();
        colorObj["b"] = cubeColor.z();
        cubeObj["color"] = colorObj;

        jsonArray.append(cubeObj);
    }
    QJsonDocument jsonDoc(jsonArray);
    file.write(jsonDoc.toJson());
    file.close();
    return true;
}

QVector3D CubeSerializer::parseVectorFromJsonObj(const QJsonObject &obj, const QStringList &prefixes)
{
    float value1 = static_cast<float>(obj[prefixes[0]].toDouble());
    float value2 = static_cast<float>(obj[prefixes[1]].toDouble());
    float value3 = static_cast<float>(obj[prefixes[2]].toDouble());

    return QVector3D(value1, value2, value3);
}
