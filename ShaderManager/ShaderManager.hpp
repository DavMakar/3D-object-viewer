#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

#include <QOpenGLShaderProgram>

class ShaderManager{
public:
    ShaderManager();
    bool loadAndCompileShaders(
        const QString& vertexShaderPath, 
        const QString& fragmentShaderPath);
    QOpenGLShaderProgram& getProgram();
private:
    QOpenGLShaderProgram program;
};

#endif //SHADER_MANAGER_HPP