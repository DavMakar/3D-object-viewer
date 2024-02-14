#include "Scene.hpp"
#include <QList>
#include <QTime>
#include <QEvent>
#include <QKeyEvent>

Scene::Scene(QWidget *parent)
    : QOpenGLWidget(parent)
    , vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    cubes.addCube({0.0f,0.0f,0.0f,{1.0f, 0.0f, 0.0f}});
    cubes.addCube({2.0f,5.0f,-15.0f,{0.0f, 1.0f, 0.0f}});
    cubes.addCube({-1.5f,-2.2f,-2.5f,{1.0f, 0.0f, 0.0f}});
    cubes.addCube({-3.8f,-2.0f,-12.3f,{1.0f, 0.0f, 0.0f}});
    cubes.addCube({2.4f,-0.4f,-3.5f,{0.0f, 0.0f, 1.0f}});
}

Scene::~Scene()
{
    vertexArrayObject.destroy();
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

    QMatrix4x4 view;
    QMatrix4x4 projection;
    
    view.translate({0.0, 0.0, -3.0});
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
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    program.release();
    update();
}

void Scene::initializeCube()
{
    // static constexpr GLfloat cubeVertices[] = {
    //     // գագաթներ            գույներ
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //      0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    //      0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    //     -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    //      0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    //     -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    // //  ․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․
    // //  ․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․
    //     -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    //      0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //      0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    //      0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f
    // };
    
    static constexpr GLfloat cubeVertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f
    };
    
    vertexArrayObject.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBuffer.allocate(cubeVertices, sizeof(cubeVertices));

//for positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

//for colors
//    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);

}

void Scene::initializeShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh")
            || !program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh")
            || !program.link()) {
        qCritical() << "Shader error";
        close();
    }
}

void Scene::onAddCubeRequest(float posX,float posY,float posZ,const QVector3D &color){
    cubes.addCube({posX,posY,posZ,color});
}
