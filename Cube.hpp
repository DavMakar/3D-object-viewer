#ifndef CUBE_HPP
#define CUBE_HPP

#include <QVector3D>

class Cube
{
public:
    Cube(float posX, float posY, float posZ);
    QVector3D getPosition() const;
private:
    float m_posX;
    float m_posY;
    float m_posZ;
};

#endif //CUBE_HPP
