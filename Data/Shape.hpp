#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <QVector3D>

class Shape
{
public:
    Shape(QString type, QVector3D pos, QVector3D color);

    QVector3D getPosition() const;
    QVector3D getColor() const;

    bool isSelected() const;
    void setSelected(bool);
    QString type() const;

protected:
    bool m_selected;
    QString m_type;
    QVector3D m_pos;
    QVector3D m_color;
};

#endif // SHAPE_HPP
