#ifndef SCENE_HPP
#define SCENE_HPP 

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "../Camera/Camera.hpp"
#include "../Data/CubeModel.hpp"
#include "../ShaderManager/ShaderManager.hpp"
#include "../Renderer/Renderer.hpp"
#include "../MousePicker/MousePicker.hpp"

class Scene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Scene(QWidget *parent = nullptr);
    ~Scene();
    void loadCubes(QList<Cube> cubeList);

public slots:
    void onAddCubeRequest(const QVector3D &posVec, const QVector3D &color);

public:
    CubeModel& getModel();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool intersects(QVector3D rayOrigin, QVector3D rayDir ,const Cube& cube);
    
    void renderShapes();
    void initializeCube();
    void initializePyramid();
    void initializeSphere();

private:
    struct Buffers {
        QOpenGLBuffer vertexBuffer{QOpenGLBuffer::VertexBuffer};
        QOpenGLBuffer indexBuffer{QOpenGLBuffer::IndexBuffer};
    };

    Camera camera;
    MousePicker mousePicker;
    CubeModel cubes;
    QList<Cube> pyramids; 
    QList<Cube> spheres; 
    ShaderManager shaderManager;
    Renderer* renderer;
    Buffers cubeBuffers, pyramidBuffers, sphereBuffers;
    QOpenGLVertexArrayObject vertexArrayObjectCube , vertexArrayObjectPyramid, vertexArrayObjectSphere;
};
#endif // SCENE_HPP
