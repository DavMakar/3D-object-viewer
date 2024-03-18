#include "MousePicker.hpp"

MousePicker::MousePicker(const Camera &camera)
    :camera(camera)
{
}

QVector3D MousePicker::raycast(QPoint mousePos, int width, int height) const
{
    QPointF normalizedMousePos = calculateNormalizedMousePos(mousePos, width ,height);
    QVector4D rayClip = QVector4D(normalizedMousePos.x(),normalizedMousePos.y(), -1.0, 1.0);
    
    QMatrix4x4 projectionMatrix = camera.getProjection(width,height);
    QVector4D rayEye = projectionMatrix.inverted() * rayClip;
    rayEye = QVector4D(rayEye.x(),rayEye.y(),-1.0 , 0.0);

    QMatrix4x4 viewMatrix = camera.getView();
    QVector3D rayWorld = (viewMatrix.inverted() * rayEye).toVector3D();
    rayWorld.normalize();
    return rayWorld;
}

QPointF MousePicker::calculateNormalizedMousePos(QPoint mousePos, int width, int height) const
{
   return QPointF(
        2.0 * mousePos.x() / width - 1.0,
        1.0 - 2.0 * mousePos.y() / height
    );
}
