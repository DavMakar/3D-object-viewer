#include "Scene.hpp"
#include <QList>
#include <QTime>
#include <QEvent>
#include <QKeyEvent>


Scene::Scene(QWidget *parent)
    : QOpenGLWidget(parent)
    , vertexBuffer(QOpenGLBuffer::VertexBuffer)
    , indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    cubes.addCube({{0.0f,0.0f,0.0f},{1.0f, 0.0f, 0.0f}});
    cubes.addCube({{2.0f,5.0f,-15.0f},{0.0f, 1.0f, 0.0f}});
    cubes.addCube({{-1.5f,-2.2f,-2.5f},{1.0f, 0.0f, 0.0f}});
    cubes.addCube({{-3.8f,-2.0f,-12.3f},{1.0f, 0.0f, 0.0f}});
    cubes.addCube({{2.4f,-0.4f,-3.5f},{0.0f, 0.0f, 1.0f}});
}

Scene::~Scene()
{
    vertexArrayObject.destroy();
}

void Scene::loadCubes(QList<Cube> cubeList)
{
    for(auto& cube: cubeList){
        cubes.addCube(cube);
    }
}

CubeModel &Scene::getModel()
{
    return cubes;
}

void Scene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    initializeCube();
    initializeShaders();
}

void Scene::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Scene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // shader program
    program.bind();
    // VAO bind;
    
    const QTime time = QTime::currentTime();

    QMatrix4x4 projection;
    QMatrix4x4 view = camera.getView();
    
    projection.perspective(45.0f, 1.0f * width() / height(), 0.1f, 100.0f);

    program.setUniformValue("view",view);
    program.setUniformValue("projection",projection);
   
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);

    for (int i = 0 ; i < cubes.rowCount(); ++i) {
        QMatrix4x4 model;
        model.translate(cubes.cubeAt(i).getPosition());

        const float angle = 20.0f * i + (time.second() * 1000 + time.msec()) / 25 % 360;
        model.rotate(angle, {0.5, 1.0, 0.0});

        program.setUniformValue("model", model);
        program.setUniformValue("ourColor", cubes.cubeAt(i).getColor());
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    
    program.release();
    update();
}

void Scene::initializeCube()
{
    static constexpr GLfloat cubeVertices[] = {
        -0.5f, 0.5f, 0.5f, //front top left
        0.5f, 0.5f, 0.5f, //front top right
        -0.5f, -0.5f, 0.5f, //front bottom left
        0.5f, -0.5f, 0.5f, //front bottom right
        -0.5f, 0.5f, -0.5f, //back top left
        0.5f, 0.5f, -0.5f, //back top right
        -0.5f, -0.5f, -0.5f, //back bottom left
        0.5f, -0.5f, -0.5f //back bottom right
    };

    static constexpr GLuint cubeIndices[] = {
        0, 2, 1, //FRONT
        1, 2, 3,
        
        0, 1, 4, //TOP
        4, 1, 5,
        
        1, 3, 5, //RIGHT
        5, 3, 7,
        
        2, 7, 3, //BOTTOM
        7, 2, 6,
        
        6, 2, 0, //LEFT
        0, 4, 6,
        
        6, 4, 5, //BACK
        5, 7, 6
    };

    vertexArrayObject.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBuffer.allocate(cubeVertices, sizeof(cubeVertices));

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    indexBuffer.allocate(cubeIndices, sizeof(cubeIndices));

//for positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

//for colors
//    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);

}

void Scene::initializeShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vshader.vsh")
            || !program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fshader.fsh")
            || !program.link()) {
        qCritical() << "Shader error";
        close();
    }
}

void Scene::onAddCubeRequest(const QVector3D& pos,const QVector3D &color){
    cubes.addCube({pos,color});
}

void Scene::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_W:
            camera.updateCameraPos(Camera::MoveDirection::FORWARD);
            break; 
        case Qt::Key_S:
            camera.updateCameraPos(Camera::MoveDirection::BACKWARD);
            break;
        case Qt::Key_A:
            camera.updateCameraPos(Camera::MoveDirection::LEFT);
            break;
        case Qt::Key_D:
            camera.updateCameraPos(Camera::MoveDirection::RIGHT);
            break;
        case Qt::Key_Up:
            camera.updateCameraPos(Camera::MoveDirection::UP);
            break;
        case Qt::Key_Down:
            camera.updateCameraPos(Camera::MoveDirection::DOWN);
            break;
        case Qt::Key_E:
            camera.updateRotation(5.0f);
            break;
        case Qt::Key_Q:
            camera.updateRotation(-5.0f);
            break;
        default:
            break;
    }
    QWidget::keyPressEvent(event);
}
