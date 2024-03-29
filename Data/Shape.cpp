#include "Shape.hpp"

int Shape::s_shapeCounter = 0; 

Shape::Shape(QString type, QVector3D pos, QVector3D color)
    : m_type(type),m_pos(pos), m_color(color) , m_selected(false)
{
    m_id = ++s_shapeCounter;
}

QVector3D Shape::getPosition() const
{
    return m_pos;
}

QVector3D Shape::getColor() const
{
    return m_color;
}

bool Shape::isSelected() const
{
    return m_selected;
}

void Shape::setSelected(bool state)
{
    m_selected = state; 
}

QString Shape::type() const
{
    return m_type;    
}

int Shape::getId() const
{
    return m_id;
}
