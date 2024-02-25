#include "Camera.hpp"

Camera::Camera()
    :m_cameraPos(0.0f,0.0f,3.0f), 
    m_worldUp(0.0f,1.0f,0.0f),
    m_cameraFront(0.0f,0.0f,-1.0f),
    m_speed(1.5f),
    m_zoom(45.0f), 
    m_pitch(0.0f),
    m_yaw(-90.0f)
{
    updateCameraVectors();
}
QMatrix4x4 Camera::getView() const{
    QMatrix4x4 view;
    view.lookAt(m_cameraPos , m_cameraPos + m_cameraFront, m_cameraUp);
    return view;
}

void Camera::updateCameraDirection(double dx,double dy){
    m_yaw += dx;
    m_pitch += dy;

    if(m_pitch > 89.0f){
        m_pitch = 89.0f;
    }else if(m_pitch < -89.0f){
        m_pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::updateCameraPos(MoveDirection dir){
    switch (dir)
    {
    case MoveDirection::FORWARD:
        m_cameraPos += m_cameraFront * m_speed;
        break;
    case MoveDirection::BACKWARD:
        m_cameraPos -= m_cameraFront * m_speed;
        break;
    case MoveDirection::RIGHT:
        m_cameraPos += m_cameraRight * m_speed;
        break;
    case MoveDirection::LEFT:
        m_cameraPos -= m_cameraRight * m_speed;
        break;
    case MoveDirection::UP:
        m_cameraPos += m_cameraUp * m_speed;
        break;
    case MoveDirection::DOWN:
        m_cameraPos -= m_cameraUp * m_speed;
        break;
    }
}

void Camera::updateCameraVectors()
{
    QVector3D direction;
    direction.setX(qCos(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    direction.setY(qSin(qDegreesToRadians(m_pitch)));
    direction.setZ(qSin(qDegreesToRadians(m_yaw))* qCos(qDegreesToRadians(m_pitch)));
    m_cameraFront = direction.normalized();

    m_cameraRight = QVector3D::crossProduct(m_cameraFront,m_worldUp);
    m_cameraUp = QVector3D::crossProduct(m_cameraRight,m_cameraFront);
}

float Camera::getPitch() const
{
    return m_pitch;
}

float Camera::getYaw() const
{
    return m_yaw;
}

float Camera::getRoll() const
{
    return m_roll;
}

void Camera::updateRotation(float angle)
{
    m_yaw += angle;
    updateCameraVectors();
}
