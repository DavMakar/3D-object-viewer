#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <QVector3D>
#include <QMatrix4x4>

class Camera{
public:
    enum class MoveDirection{
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    Camera();
    float getPitch() const;
    float getYaw() const;
    float getRoll() const;
    QMatrix4x4 getView() const;
    QMatrix4x4 getProjection(int width , int height) const;
    QVector3D getPosition() const;

    void updateCameraDirection(double dx,double dy);
    void updateCameraPos(float deltaTime);
    void updateCameraVectors();
    void updateRotation(float angle);
    void move(MoveDirection dir, bool moving);

private:
    QHash<MoveDirection, bool> moveDirections = {
        {MoveDirection::FORWARD, false},
        {MoveDirection::BACKWARD, false},
        {MoveDirection::LEFT, false},
        {MoveDirection::RIGHT, false},
        {MoveDirection::UP, false},
        {MoveDirection::DOWN, false},
    };
    
    QVector3D m_cameraPos;

    QVector3D m_worldUp;
    QVector3D m_cameraFront;
    QVector3D m_cameraUp;
    QVector3D m_cameraRight;

    float m_speed;
    float m_zoom;
    float m_sensitivity;

    float m_pitch; // rotation y
    float m_yaw; // z
    float m_roll; // x
    float m_fov;
    // float m_nearPlane = 0.1f;
    // float m_farPlane = 100;
};

#endif //CAMERA_HPP
