#ifndef RENDERER_HPP
#define RENDERER_HPP

class QOpenGLShaderProgram;
class QOpenGLVertexArrayObject;

#include <QOpenGLFunctions>
#include "../Data/Shape.hpp"

class Renderer: protected QOpenGLFunctions{
public:
    Renderer(QOpenGLShaderProgram& program, 
     QOpenGLVertexArrayObject& vaoGrid,
     QOpenGLVertexArrayObject& vaoCube,
     QOpenGLVertexArrayObject& vaoPyramid,
     QOpenGLVertexArrayObject& vaoSphere);
    void drawScene(const QList<Shape>& shapes);

private:
    void renderCube(const Shape& shape);
    void renderPyramid(const Shape& shape);
    void renderSphere(const Shape& shape);
    void drawGrid();

private:
    QOpenGLShaderProgram& program;
    QOpenGLVertexArrayObject& vaoGrid;
    QOpenGLVertexArrayObject& vaoCube;
    QOpenGLVertexArrayObject& vaoPyramid;
    QOpenGLVertexArrayObject& vaoSphere;
};

#endif //RENDERER_HPP
