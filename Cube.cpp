#include "Cube.hpp"

Cube::Cube(float posX, float posY, float posZ)
    :m_posX(posX),m_posY(posY),m_posZ(posZ)
{
}

QVector3D Cube::getPosition() const
{
    return QVector3D(m_posX,m_posY,m_posZ);
}
