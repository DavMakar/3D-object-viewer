#ifndef CUBE_HPP
#define CUBE_HPP

#include <QVector3D>

class Cube
{
public:
    Cube(QVector3D pos, QVector3D color);
    QVector3D getPosition() const;
    QVector3D getColor() const;
    bool isSelected() const;
    void setSelected(bool);
private:
    QVector3D m_pos;
    QVector3D m_color;
    bool m_selected;
};

#endif //CUBE_HPP
