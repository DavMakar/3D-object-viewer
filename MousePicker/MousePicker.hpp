#ifndef MOUSEPICKER_HPP
#define MOUSEPICKER_HPP

#include <QPointF>
#include <QVector3D>
#include "../Camera/Camera.hpp"

class MousePicker {
public:
    MousePicker(const Camera& camera);
    QVector3D raycast(QPoint mousePos, int width, int height) const;

private:
    QPointF calculateNormalizedMousePos(QPoint mousePos, int width, int height) const;

private:
    const Camera& camera;
};

#endif //MOUSEPICKER_HPP