#include "ShaderManager.hpp"

ShaderManager::ShaderManager()
    :program()
{
}

bool ShaderManager::loadAndCompileShaders(const QString & vertexShaderPath, const QString & fragmentShaderPath)
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath)
            || !program.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath)
            || !program.link()) {
        qCritical() << "Shader error:" << program.log();
        return false;
    }
    return true;
}

QOpenGLShaderProgram &ShaderManager::getProgram()
{
    return program;
}
