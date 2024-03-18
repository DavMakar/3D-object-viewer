#ifndef RENDERER_HPP
#define RENDERER_HPP


class Cube;
class CubeModel;
class QOpenGLShaderProgram;
class QOpenGLVertexArrayObject;

#include <QOpenGLFunctions>

class Renderer: protected QOpenGLFunctions{
public:
    Renderer(QOpenGLShaderProgram& program, QOpenGLVertexArrayObject& cubeVAO, QOpenGLVertexArrayObject& pyramidVAO);
    void renderCubes(CubeModel& cubes);
    void renderPyramid(const Cube& pyramid);


private:
    QOpenGLShaderProgram& program;
    QOpenGLVertexArrayObject& cubeVAO;
    QOpenGLVertexArrayObject& pyramidVAO;
};

#endif //RENDERER_HPP
