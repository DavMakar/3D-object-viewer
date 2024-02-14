#ifndef CUBE_HPP
#define CUBE_HPP

#include <QVector3D>

class Cube
{
public:
    Cube(float posX, float posY, float posZ, QVector3D color);
    QVector3D getPosition() const;
    QVector3D getColor() const;
private:
    float m_posX;
    float m_posY;
    float m_posZ;
    QVector3D m_color;

};

#endif //CUBE_HPP
