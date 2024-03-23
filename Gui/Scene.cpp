#include "Scene.hpp"
#include <QList>
#include <QTime>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

Scene::Scene(QWidget *parent)
    : QOpenGLWidget(parent)
    , mousePicker(camera)
    , renderer(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
}

Scene::~Scene()
{
    vertexArrayObjectCube.destroy();
    vertexArrayObjectPyramid.destroy();
    delete renderer;
}

void Scene::loadShapes(const QList<Shape>& shapeList)
{  
    for(auto& shape : shapeList){
        shapes.addShape(shape);
    }
}

ShapeModel &Scene::getModel()
{
    return shapes;
}

void Scene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    initializeCube();
    initializePyramid();
    initializeSphere();
    if(!shaderManager.loadAndCompileShaders(":/Shaders/vshader.vsh", ":/Shaders/fshader.fsh")){
        close();
    };
}

void Scene::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Scene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // shader program
    shaderManager.getProgram().bind();
    // VAO bind;
    
    QMatrix4x4 view = camera.getView();
    QMatrix4x4 projection = camera.getProjection(width(), height());
    
    shaderManager.getProgram().setUniformValue("view",view);
    shaderManager.getProgram().setUniformValue("projection",projection);
    
    if(renderer == nullptr)
        renderer = new Renderer(shaderManager.getProgram(),vertexArrayObjectCube,vertexArrayObjectPyramid,vertexArrayObjectSphere);
    
    renderer->drawScene(shapes.getShapes());

    shaderManager.getProgram().release();
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

    cubeBuffers.vertexBuffer.create();
    cubeBuffers.vertexBuffer.bind();
    cubeBuffers.vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    cubeBuffers.vertexBuffer.allocate(cubeVertices, sizeof(cubeVertices));

    cubeBuffers.indexBuffer.create();
    cubeBuffers.indexBuffer.bind();
    cubeBuffers.indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    cubeBuffers.indexBuffer.allocate(cubeIndices, sizeof(cubeIndices));

//for positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
}

void Scene::initializePyramid()
{
    static constexpr GLfloat pyramidVertices[] =
    { 
       -0.5f, -0.5f, 0.5f,
       0.5f, -0.5f, 0.5f, 
       0.0f, 0.5f, 0.0f,  
       0.5f, -0.5f, 0.5f, 
       0.5f, -0.5f, -0.5f,
       0.0f, 0.5f, 0.0f,  
       0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
       0.0f, 0.5f, 0.0f,  
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, 0.5f, 
       0.0f, 0.5f, 0.0f,  
      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f,  0.5f, 
      -0.5f, -0.5f,  0.5f, 
       0.5f, -0.5f,  0.5f, 
      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f
    };

    vertexArrayObjectPyramid.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObjectPyramid);

    pyramidBuffers.vertexBuffer.create();
    pyramidBuffers.vertexBuffer.bind();
    pyramidBuffers.vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    pyramidBuffers.vertexBuffer.allocate(pyramidVertices, sizeof(pyramidVertices));

//for positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

}

void Scene::initializeSphere()
{
    std::vector<float> sphereVertices;
    std::vector<int> sphereIndices;
    const int stacks = 20; 
    const int slices = 20; 
    const float radius = 0.2f;

    const float dTheta = -2 * M_PI / float(slices);
    const float dPhi = -1 * M_PI / float(stacks);

    for (int i = 0; i <= stacks; ++i)
    {
        QMatrix4x4 rotMatrix;
        rotMatrix.rotate(dPhi*float(i),QVector3D(0.0f,0.0f,1.0f));
        QVector3D stackStartPoint = QVector3D(rotMatrix * QVector4D(0.0f,-radius,0.0f,1.0f));
        for (int j = 0; j <= slices; ++j)
        {
            rotMatrix.rotate(dTheta * float(j),QVector3D(0.0f,1.0f,0.0f));
            QVector3D pos = QVector3D(rotMatrix * QVector4D(stackStartPoint, 1.0f));
            sphereVertices.push_back(pos.x());
            sphereVertices.push_back(pos.y());
            sphereVertices.push_back(pos.z());
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        const int offset = (slices + 1) * i;

        for (int j = 0; j < slices; ++j)
        {
            sphereIndices.push_back(offset + i);
            sphereIndices.push_back(offset + i + slices + 1);
            sphereIndices.push_back(offset + i + 1 + slices + 1);

            sphereIndices.push_back(offset + i);
            sphereIndices.push_back(offset + i + 1 +slices + 1);
            sphereIndices.push_back(offset + i + 1);
        }
    }

    // Initialize vertex buffer
    sphereBuffers.vertexBuffer.create();
    sphereBuffers.vertexBuffer.bind();
    sphereBuffers.vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    sphereBuffers.vertexBuffer.allocate(sphereVertices.data(), sphereVertices.size() * sizeof(float));

    // Initialize index buffer
    qDebug() << "sphereIdx " << sphereIndices.size();
    sphereBuffers.indexBuffer.create();
    sphereBuffers.indexBuffer.bind();
    sphereBuffers.indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    sphereBuffers.indexBuffer.allocate(sphereIndices.data(), sphereIndices.size() * sizeof(unsigned int));

    // Set vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
}

void Scene::onAddShapeRequest(const QString& type,const QVector3D& pos,const QVector3D &color){
    shapes.addShape({type,pos,color});
}

void Scene::mousePressEvent(QMouseEvent *event){
    auto rayWorld = mousePicker.raycast(event->pos(),width(),height());
    
    qDebug() << "ray world " << rayWorld;

    auto cameraPos = camera.getPosition();
    qDebug() << "camera Pos " << cameraPos;

    for(auto& shape : shapes){
        shape.setSelected(false);
    }

    for(auto& shape : shapes){
        if(intersects(cameraPos, rayWorld, shape)){
            qDebug() << "selected";
            shape.setSelected(true);
        }
    }
}

bool Scene::intersects(QVector3D rayOrigin, QVector3D rayDir, const Shape& shape) {
    QMatrix4x4 model;
    model.translate(shape.getPosition());

    QMatrix4x4 modelInverse = model.inverted();
    
    QVector4D rayOrigin4D = modelInverse * QVector4D(rayOrigin.x(), rayOrigin.y(), rayOrigin.z(), 1.0f);
    QVector4D rayDir4D = modelInverse * QVector4D(rayDir.x(), rayDir.y(), rayDir.z(), 1.0f);

    rayOrigin = rayOrigin4D.toVector3D();
    rayDir = rayDir4D.toVector3D();

    QVector3D bounds[2];
    bounds[0] = QVector3D(-0.5f, -0.5f, -0.5f);
    bounds[1] = QVector3D(0.5f, 0.5f, 0.5f);

    float txmin, txmax, tymin, tymax, tzmin, tzmax;

    txmin = (bounds[0].x() - rayOrigin.x()) / rayDir.x();
    txmax = (bounds[1].x() - rayOrigin.x()) / rayDir.x();
    tymin = (bounds[0].y() - rayOrigin.y()) / rayDir.y();
    tymax = (bounds[1].y() - rayOrigin.y()) / rayDir.y();
    tzmin = (bounds[0].z() - rayOrigin.z()) / rayDir.z();
    tzmax = (bounds[1].z() - rayOrigin.z()) / rayDir.z();

    float tMin = std::max(std::max(std::min(txmin, txmax), std::min(tymin, tymax)), std::min(tymin,tymax));
    float tMax = std::min(std::min(std::max(txmin, txmax), std::max(tymin, tymax)), std::max(tymin, tymax));

    return tMax >= tMin && tMax >= 0;
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

// void Scene::computeRay(const QPoint& mousePos, QVector3D& rayOrigin , QVector3D& rayDirection){
//     QPointF normalizedMousePos = QPointF(
//         2.0 * mousePos.x() / width() - 1.0,
//         1.0 - 2.0 * mousePos.y() / height()
//     );

//     QMatrix4x4 viewMatrix = camera.getView();
//     QVector4D nearPlanePoint = QVector4D(normalizedMousePos.x(), normalizedMousePos.y(), -1.0, 1.0);
//     QVector4D farPlanePoint = QVector4D(normalizedMousePos.x(), normalizedMousePos.y(), 1.0, 1.0);

//     QVector4D nearWorldPoint = viewMatrix.inverted() * nearPlanePoint;
//     QVector4D farWorldPoint = viewMatrix.inverted() * farPlanePoint;

//     rayOrigin = QVector3D(nearWorldPoint.x(), nearWorldPoint.y(), nearWorldPoint.z());
//     rayDirection = (QVector3D(farWorldPoint) - QVector3D(nearWorldPoint)).normalized();
// }
