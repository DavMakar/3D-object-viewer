#include "Renderer.hpp"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "../Data/CubeModel.hpp"
#include "../Data/Cube.hpp"

Renderer::Renderer(QOpenGLShaderProgram &program, QOpenGLVertexArrayObject &cubeVAO, QOpenGLVertexArrayObject &pyramidVAO)
    :program(program) , cubeVAO(cubeVAO) , pyramidVAO(pyramidVAO)
{
    initializeOpenGLFunctions();
}

void Renderer::renderCubes(CubeModel &cubes)
{
    for (auto& cube : cubes) {
        QOpenGLVertexArrayObject::Binder vaoBinder(&cubeVAO);
        QMatrix4x4 model;
        model.translate(cube.getPosition());
        program.setUniformValue("model", model);
        if (cube.isSelected()) {
            program.setUniformValue("ourColor", QVector3D(1.0f, 1.0f, 0.0f));
        } else {
            program.setUniformValue("ourColor", cube.getColor());
        }
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

void Renderer::renderPyramid(const Cube& pyramid)
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&cubeVAO);
    QMatrix4x4 model;
    model.translate(pyramid.getPosition());

    program.setUniformValue("model", model);
    program.setUniformValue("ourColor", pyramid.getColor());
    glDrawArrays(GL_TRIANGLES, 0, 18);
}
