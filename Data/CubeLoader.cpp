#include "CubeLoader.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QList<Cube> CubeLoader::loadCubesFromJson(const QString &filename)
{
    QList<Cube> cubeList;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<< "aaa";
        return cubeList;
    };
    QJsonDocument jsonDoc =  QJsonDocument::fromJson(file.readAll());
    
    if (!jsonDoc.isArray())
    {
        qDebug() << "Invalid JSON format. Expected array.";
        return cubeList;
    }
    
    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue& value : jsonArray)
    {
        if (!value.isObject())
        {
            qDebug() << "Invalid JSON format. Expected object.";
            continue;
        }

        QJsonObject jsonObj = value.toObject();
        QJsonObject positionObj = jsonObj["position"].toObject();
        QJsonObject colorObj = jsonObj["color"].toObject();

        auto posVec = parseVectorFromJsonObj(positionObj,{"x","y","z"});
        auto colorVec = parseVectorFromJsonObj(colorObj,{"r","g","b"});

        cubeList.append({posVec, colorVec});
    }

    file.close();
    return cubeList;
}


QVector3D CubeLoader::parseVectorFromJsonObj(const QJsonObject &obj, const QStringList &prefixes)
{
    float value1 = static_cast<float>(obj[prefixes[0]].toDouble());
    float value2 = static_cast<float>(obj[prefixes[1]].toDouble());
    float value3 = static_cast<float>(obj[prefixes[2]].toDouble());

    return QVector3D(value1,value2,value3);
}
