#ifndef SCENE_HPP
#define SCENE_HPP 

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "../Data/CubeModel.hpp"
#include "../Camera/Camera.hpp"

class Scene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Scene(QWidget *parent = nullptr);
    ~Scene();
    void loadCubes(QList<Cube> cubeList);

public slots:
    void onAddCubeRequest(const QVector3D &posVec, const QVector3D &color);
    CubeModel& getModel();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    void initializeCube();
    void initializeShaders();

private:
    Camera camera;
    CubeModel cubes;
    QOpenGLBuffer vertexBuffer , indexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLShaderProgram program;
};
#endif // SCENE_HPP
