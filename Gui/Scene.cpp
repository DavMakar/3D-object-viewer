#include "Scene.hpp"
#include <QList>
#include <QTime>
#include <QEvent>
#include <QKeyEvent>


Scene::Scene(QWidget *parent)
    : QOpenGLWidget(parent)
    , vertexBufferCube(QOpenGLBuffer::VertexBuffer)
    , vertexBufferPyramid(QOpenGLBuffer::VertexBuffer)
    , indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    setFocusPolicy(Qt::StrongFocus);
    cubes.addCube({{0.0f,0.0f,0.0f},{1.0f, 0.0f, 0.0f}});
    cubes.addCube({{6.0f,0.0f,2.0f},{0.0f, 1.0f, 0.0f}});
    cubes.addCube({{3.5f,4.0f,0.0f},{0.0f, 0.0f, 1.0f}});
    cubes.addCube({{-5.5f,1.0f,4.0f},{1.0f, 0.0f, 1.0f}});
    cubes.addCube({{1.5f,-1.0f,0.0f},{0.0f, 1.0f, 1.0f}});
    cubes.addCube({{-1.0f,-5.0f,0.0f},{0.0f, 0.5f, 1.0f}});
}

Scene::~Scene()
{
    vertexArrayObjectCube.destroy();
    vertexArrayObjectPyramid.destroy();
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
    initializePyramid();
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
    
    {
        QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObjectCube);
        int i = 0;
        for (const auto& cube: cubes) {
            QMatrix4x4 model;
            model.translate(cube.getPosition());
            
            if(i % 3 == 0)
                model.scale({3.0f,3.0f,3.0f});
            else if(i % 2 == 0)
                model.scale({2.0f,2.0f,2.0f});
                
            const float angle = 20.0f * i + (time.second() * 1000 + time.msec()) / 25 % 360;
            model.rotate(angle, {0.5, 1.0, 0.0});

            program.setUniformValue("model", model);

            if (cube.isSelected()) {
                program.setUniformValue("ourColor", QVector3D(1.0f, 1.0f, 0.0f)); 
            } else {
                program.setUniformValue("ourColor", cube.getColor());
            }
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            ++i;
        }
    }
    {
        QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObjectPyramid);
        QList<Cube> pyramids;
        pyramids.push_back({{2.0f,4.0f,6.0f},{1.0f,0.2f,0.4f}});
        pyramids.push_back({{-2.0f,2.0f,-3.0f},{0.5f,0.4f,0.4f}});

        for(auto& pyramid : pyramids){
            QMatrix4x4 model;
            model.translate(pyramid.getPosition());
            model.scale({3.0f,3.0f,3.0f});
                
            program.setUniformValue("model", model);
            program.setUniformValue("ourColor", pyramid.getColor());
            glDrawArrays(GL_TRIANGLES,0, 18);
        }
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

    vertexArrayObjectCube.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObjectCube);

    vertexBufferCube.create();
    vertexBufferCube.bind();
    vertexBufferCube.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBufferCube.allocate(cubeVertices, sizeof(cubeVertices));

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

void Scene::initializePyramid()
{
    static constexpr GLfloat pyramidVertices[] =
    { 
       -0.2f, -0.2f, 0.2f,
       0.2f, -0.2f, 0.2f, 
       0.0f, 0.2f, 0.0f,  
       0.2f, -0.2f, 0.2f, 
       0.2f, -0.2f, -0.2f,
       0.0f, 0.2f, 0.0f,  
       0.2f, -0.2f, -0.2f,
      -0.2f, -0.2f, -0.2f,
       0.0f, 0.2f, 0.0f,  
      -0.2f, -0.2f, -0.2f,
      -0.2f, -0.2f, 0.2f, 
       0.0f, 0.2f, 0.0f,  
      -0.2f, -0.2f, -0.2f,
       0.2f, -0.2f,  0.2f, 
      -0.2f, -0.2f,  0.2f, 
       0.2f, -0.2f,  0.2f, 
      -0.2f, -0.2f, -0.2f,
       0.2f, -0.2f, -0.2f
    };

    vertexArrayObjectPyramid.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObjectPyramid);

    vertexBufferPyramid.create();
    vertexBufferPyramid.bind();
    vertexBufferPyramid.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBufferPyramid.allocate(pyramidVertices, sizeof(pyramidVertices));

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
