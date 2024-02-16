#include "Cube.hpp"

Cube::Cube(QVector3D pos, QVector3D color)
    :m_pos(pos),m_color(color)
{
}

QVector3D Cube::getPosition() const
{
    return m_pos;
}

QVector3D Cube::getColor() const
{
    return m_color;
}
