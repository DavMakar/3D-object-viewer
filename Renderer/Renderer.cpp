#include "Renderer.hpp"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "../Data/Shape.hpp"
#include "../Data/ShapeModel.hpp"

Renderer::Renderer(QOpenGLShaderProgram &program,
  QOpenGLVertexArrayObject &vaoGrid,
  QOpenGLVertexArrayObject &vaoCube,
  QOpenGLVertexArrayObject &vaoPyramid,
  QOpenGLVertexArrayObject &vaoSphere)
    :program(program) , vaoGrid(vaoGrid) , vaoCube(vaoCube) , vaoPyramid(vaoPyramid), vaoSphere(vaoSphere)
{
    initializeOpenGLFunctions();
}

void Renderer::drawScene(const QList<Shape>& shapes)
{
    drawGrid();
    for(const auto& shape : shapes){
        auto type = shape.type();
        if(type == "Cube"){
            renderCube(shape);
        }
        else if(type == "Sphere"){
            renderSphere(shape);
        }
        else if(type == "Pyramid"){
            renderPyramid(shape);
        }
    }
}

void Renderer::renderCube(const Shape& cube)
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoCube);
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

void Renderer::renderPyramid(const Shape& pyramid)
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoPyramid);
    QMatrix4x4 model;
    model.translate(pyramid.getPosition());

    program.setUniformValue("model", model);
    program.setUniformValue("ourColor", pyramid.getColor());
    glDrawArrays(GL_TRIANGLES, 0, 18);
}

void Renderer::renderSphere(const Shape& sphere)
{
   QOpenGLVertexArrayObject::Binder vaoBinder(&vaoSphere);
   QMatrix4x4 model;
   model.translate(sphere.getPosition());
   program.setUniformValue("model", model);
   if (sphere.isSelected()) {
       program.setUniformValue("ourColor", QVector3D(1.0f, 1.0f, 0.0f));
   } else {
       program.setUniformValue("ourColor", sphere.getColor());
   }
   glDrawElements(GL_TRIANGLES, 2400, GL_UNSIGNED_INT, 0);
}

void Renderer::drawGrid()
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoGrid);
    QMatrix4x4 model;
    model.translate({0.0,0.0,0.0});
    program.setUniformValue("model", model);
    program.setUniformValue("ourColor", QVector3D(1.0f, 1.0f, 1.0f));
    glDrawElements(GL_LINES, 3200, GL_UNSIGNED_INT, 0);
}
