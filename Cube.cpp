#include "Cube.hpp"

Cube::Cube(float posX, float posY, float posZ, QVector3D color)
    :m_posX(posX),m_posY(posY),m_posZ(posZ),m_color(color)
{
}

QVector3D Cube::getPosition() const
{
    return QVector3D(m_posX,m_posY,m_posZ);
}

QVector3D Cube::getColor() const
{
    return m_color;
}
