#pragma once

#include "GLCommon.h"
#include "GLShader.h"

class GLProgram
{
public:
    GLProgram();

    virtual bool Create();
    virtual void Release();
    virtual void Use();

    void Attach(const GLShader&);
    void Detach(const GLShader&);
    bool Link(std::string& log);
    bool Link();

    void  BindAttribLocation(GLuint index, const std::string& name);
    GLint GetAttribLocation(const std::string& name);
    void  BindFragDataLocation(GLuint index, const std::string& name);
    GLint GetFragDataLocation(const std::string& name);

    bool Uniform1i(const std::string& name, int value);
    bool UniformV3f(const std::string& name, const Vector<float, 3>& value);
    bool UniformV4f(const std::string& name, const Vector<float, 4>& value);
    bool UniformM4f(const std::string& name, const Matrix<float, 4>& value);

    operator bool() const
    {
        return !!this->program;
    }
    operator GLuint() const
    {
        return this->program;
    }

private:
    GLint UniformLocInUse(const std::string& name);

protected:
    GLuint program;
};