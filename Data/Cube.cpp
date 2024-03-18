#include "Cube.hpp"

Cube::Cube(QVector3D pos, QVector3D color)
    :m_pos(pos),m_color(color) ,m_size({1.0,1.0,1.0}), m_selected(false)
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

QVector3D Cube::getSize() const
{
    return m_size;
}

bool Cube::isSelected() const
{
    return m_selected;
}

void Cube::setSelected(bool selected)
{
    m_selected = selected; 
}
