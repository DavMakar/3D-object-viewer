#ifndef RENDERER_HPP
#define RENDERER_HPP


class QOpenGLShaderProgram;
class QOpenGLVertexArrayObject;

#include <QOpenGLFunctions>
#include "../Data/Shape.hpp"

class Renderer: protected QOpenGLFunctions{
public:
    Renderer(QOpenGLShaderProgram& program, QOpenGLVertexArrayObject& cubeVAO,
     QOpenGLVertexArrayObject& pyramidVAO);
    void drawScene(const QList<Shape>& shapes);

private:
    void renderCube(const Shape& shape);
    void renderPyramid(const Shape& shape);
   // void renderSphere(const Shape& shape);

private:
    QOpenGLShaderProgram& program;
    QOpenGLVertexArrayObject& cubeVAO;
    QOpenGLVertexArrayObject& pyramidVAO;
};

#endif //RENDERER_HPP
