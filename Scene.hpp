#ifndef SCENE_HPP
#define SCENE_HPP 

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "Cube.hpp"

class Scene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Scene(QWidget *parent = nullptr);
    ~Scene();

public slots:
    void onAddCubeRequest(float posX,float posY,float posZ,const QVector3D &color);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void initializeCube();
    void initializeShaders();
    QVector<Cube> cubes;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLShaderProgram program;
};
#endif // SCENE_HPP
