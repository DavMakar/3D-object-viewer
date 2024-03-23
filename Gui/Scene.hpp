#ifndef SCENE_HPP
#define SCENE_HPP 

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "../Camera/Camera.hpp"
#include "../Data/ShapeModel.hpp"
#include "../ShaderManager/ShaderManager.hpp"
#include "../Renderer/Renderer.hpp"
#include "../MousePicker/MousePicker.hpp"

class Scene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Scene(QWidget *parent = nullptr);
    ~Scene();
    void loadShapes(const QList<Shape>& shapes);

public slots:
    void onAddShapeRequest(const QString& type, const QVector3D &posVec, const QVector3D &color);
    void onClearSceneAction();

public:
    ShapeModel& getModel();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool intersects(QVector3D rayOrigin, QVector3D rayDir ,const Shape& shape);
    
    void initializeGrid();
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
    ShapeModel shapes; 
    ShaderManager shaderManager;
    Renderer* renderer;
    Buffers gridBuffers , cubeBuffers, pyramidBuffers, sphereBuffers;
    QOpenGLVertexArrayObject vaoGrid, vaoCube , vaoPyramid, vaoSphere;
};
#endif // SCENE_HPP
