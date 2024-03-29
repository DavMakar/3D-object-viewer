#include "Scene.hpp"
#include <QList>
#include <QTime>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

Scene::Scene(QWidget *parent)
    : QOpenGLWidget(parent)
    , mode(Mode::MOVE)
    , mousePicker(camera)
    , renderer(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
}

Scene::~Scene()
{
    vaoCube.destroy();
    vaoPyramid.destroy();
    delete renderer;
}

void Scene::loadShapes(const QList<Shape>& shapeList)
{  
    for(auto& shape : shapeList){
        shapes.addShape(shape);
    }
}

void Scene::onClearSceneAction()
{
    shapes.removeAllShapes();
}

void Scene::setMode(Mode mode)
{
    this->mode = mode;
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

    initializeGrid();
    initializeCube();
    initializePyramid();
    initializeSphere();
    if(!shaderManager.loadAndCompileShaders(":/Shaders/vshader.vsh", ":/Shaders/fshader.fsh")){
        close();
    };

    timer.start();
    lastTime=(GLfloat)timer.elapsed()/1000;
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
    
    float currTime=(float)timer.elapsed()/1000;
    float deltaTime=currTime-lastTime;
    lastTime=currTime;

    camera.updateCameraPos(deltaTime);
    camera.updateCameraDirection(getXoffset(),getYoffset());
    QMatrix4x4 view = camera.getView();
    QMatrix4x4 projection = camera.getProjection(width(), height());
    
    shaderManager.getProgram().setUniformValue("view",view);
    shaderManager.getProgram().setUniformValue("projection",projection);
    
    if(renderer == nullptr)
        renderer = new Renderer(shaderManager.getProgram(),vaoGrid, vaoCube,vaoPyramid,vaoSphere);
    
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

    vaoCube.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoCube);

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

    vaoPyramid.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoPyramid);

    pyramidBuffers.vertexBuffer.create();
    pyramidBuffers.vertexBuffer.bind();
    pyramidBuffers.vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    pyramidBuffers.vertexBuffer.allocate(pyramidVertices, sizeof(pyramidVertices));

//for positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

}

void Scene::initializeGrid(){
    std::vector<float> gridVertices;
    std::vector<unsigned int> gridIndices;
    const int slices = 20;

    for(int i = 0; i <= slices; ++i) {
        float z = (float)i / (float)slices;
        for(int j = 0; j <= slices; ++j) {
            float x = (float)j / (float)slices;
            float y = 0;
            gridVertices.push_back(x);
            gridVertices.push_back(y);
            gridVertices.push_back(z);
        }
    }

    for(int i = 0; i < slices; ++i) {
        int row1 =  i * (slices+1);
        int row2 = (i+1) * (slices+1);
        for(int j = 0; j < slices; ++j) {
            gridIndices.push_back(row1+j);
            gridIndices.push_back(row1+j+1);
            gridIndices.push_back(row1+j+1);
            gridIndices.push_back(row2+j+1);

            gridIndices.push_back(row2+j+1);
            gridIndices.push_back(row2+j);
            gridIndices.push_back(row2+j);
            gridIndices.push_back(row1+j);
        }
    }

    vaoGrid.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoGrid);
    
    gridBuffers.vertexBuffer.create();
    gridBuffers.vertexBuffer.bind();
    gridBuffers.vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    gridBuffers.vertexBuffer.allocate(gridVertices.data(), gridVertices.size() * sizeof(float));

    qDebug() << "gridIdx " << gridIndices.size();
    gridBuffers.indexBuffer.create();
    gridBuffers.indexBuffer.bind();
    gridBuffers.indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    gridBuffers.indexBuffer.allocate(gridIndices.data(), gridIndices.size() * sizeof(unsigned int));

    // Set vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
}

void Scene::initializeSphere()
{
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    const int stacks = 20;
    const int slices = 20;
    const float radius = 0.5f;

    const float dTheta = -2 * M_PI / float(slices);
    const float dPhi = 2 * M_PI / float(stacks);

    for (int i = 0; i <= stacks; ++i)
    {
         for (int j = 0; j <= slices; ++j)
        {
            QVector3D pos;
            pos[0] = radius* qCos(dTheta* i) * qCos(j*dPhi);
            pos[1] = radius* qSin(dTheta* i);
            pos[2] = radius* qCos(dTheta* i)* qSin(j*dPhi);

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
            sphereIndices.push_back(offset + j); //k1 
            sphereIndices.push_back(offset + j + slices + 1); //k2
            sphereIndices.push_back(offset + j + 1); //k1 +1 

            sphereIndices.push_back(offset + j + 1); //k1+1
            sphereIndices.push_back(offset + j + slices + 1); //k2
            sphereIndices.push_back(offset + j + slices + 2); //k2+1
        }
    }

    vaoSphere.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoSphere);
    
    sphereBuffers.vertexBuffer.create();
    sphereBuffers.vertexBuffer.bind();
    sphereBuffers.vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    sphereBuffers.vertexBuffer.allocate(sphereVertices.data(), sphereVertices.size() * sizeof(float));

    qDebug() << "sphereIdx " << sphereIndices.size();
    sphereBuffers.indexBuffer.create();
    sphereBuffers.indexBuffer.bind();
    sphereBuffers.indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    sphereBuffers.indexBuffer.allocate(sphereIndices.data(), sphereIndices.size() * sizeof(unsigned int));

    // Set vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
}

int Scene::getXoffset()
{
    int tmp = xoffset;
    xoffset = 0;
    return tmp;
}

int Scene::getYoffset()
{
    int tmp = yoffset;
    yoffset = 0;
    return tmp;
}

void Scene::onAddShapeRequest(const QString& type,const QVector3D& pos,const QVector3D &color){
    shapes.addShape({type,pos,color});
}

void Scene::mousePressEvent(QMouseEvent *event){
    if(mode == Mode::SELECT){
        auto rayWorld = mousePicker.raycast(event->pos(),width(),height());
        
        qDebug() << "ray world " << rayWorld;

        auto cameraPos = camera.getPosition();
        qDebug() << "camera Pos " << cameraPos;

        for(auto& shape : shapes){
            shape.setSelected(false);
        }

        for(auto& shape: shapes){
            if(raySphere(cameraPos, rayWorld, shape.getPosition(), 0.5f)){
                shape.setSelected(true);
            }
        }

        // for(auto& shape : shapes){
        //     if(intersects(cameraPos, rayWorld, shape)){
        //         qDebug() << "selected";
        //         shape.setSelected(true);
        //     }
        // }
    }
}

void Scene::mouseMoveEvent(QMouseEvent * event)
{
    if(mode == Mode::MOVE){
        static QPointF lastPos = event->pos(); //TRY make member
        QPointF currentPos = event->pos();

        xoffset = currentPos.x() - lastPos.x();
        yoffset = lastPos.y() - currentPos.y();

        qDebug()<<"xoff = "<< xoffset << " yoff = " << yoffset;

        lastPos = currentPos;
    }
    QWidget::mouseMoveEvent(event);
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

bool Scene::raySphere(QVector3D rayOrigin, QVector3D rayDir, QVector3D sphereCenterPos, float radius)
{
    QVector3D distToSphere = rayOrigin - sphereCenterPos; 
    GLfloat b = QVector3D::dotProduct(rayDir, distToSphere);
    GLfloat c = QVector3D::dotProduct(distToSphere,distToSphere) - radius * radius;
    GLfloat bSquaredMinusC = b * b - c;
    
    if (bSquaredMinusC < 0.0f) {
		return false;
	}
    else if(bSquaredMinusC == 0.0f){
        GLfloat t = -b;
        if(t < 0.0f){
            return false;
        }
        return true;
    }
    else{
        GLfloat t1 = -b + sqrt(bSquaredMinusC);
		GLfloat t2 = -b - sqrt(bSquaredMinusC);
        if(t1 < 0.0f && t2 < 0.0f){
            return false;
        }
        else{
            return true;
        }
    }
}

void Scene::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_W:
            camera.move(Camera::MoveDirection::FORWARD,true);
            break; 
        case Qt::Key_S:
            camera.move(Camera::MoveDirection::BACKWARD,true);
            break;
        case Qt::Key_A:
            camera.move(Camera::MoveDirection::LEFT,true);
            break;
        case Qt::Key_D:
            camera.move(Camera::MoveDirection::RIGHT,true);
            break;
        case Qt::Key_Up:
            camera.move(Camera::MoveDirection::UP,true);
            break;
        case Qt::Key_Down:
            camera.move(Camera::MoveDirection::DOWN,true);
            break;
        default:
            break;
    }
    QWidget::keyPressEvent(event);
}

void Scene::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()){
        case Qt::Key_W:
            camera.move(Camera::MoveDirection::FORWARD,false);
            break; 
        case Qt::Key_S:
            camera.move(Camera::MoveDirection::BACKWARD,false);
            break;
        case Qt::Key_A:
            camera.move(Camera::MoveDirection::LEFT,false);
            break;
        case Qt::Key_D:
            camera.move(Camera::MoveDirection::RIGHT,false);
            break;
        case Qt::Key_Up:
            camera.move(Camera::MoveDirection::UP,false);
            break;
        case Qt::Key_Down:
            camera.move(Camera::MoveDirection::DOWN,false);
            break;
        default:
            break;
    }
    QWidget::keyReleaseEvent(event);
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
