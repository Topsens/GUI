#pragma once

#include "GLCommon.h"
#include "GLShader.h"

class GLProgram
{
public:
    GLProgram();

    virtual bool Create();
    virtual void Release();

    void Attach(const GLShader&);
    void Detach(const GLShader&);
    bool Link(std::string& log);
    bool Link();
    void Use();

    void  BindAttribLocation(GLuint index, const std::string& name);
    GLint GetAttribLocation(const std::string& name);
    void  BindFragDataLocation(GLuint index, const std::string& name);
    GLint GetFragDataLocation(const std::string& name);

    operator bool() const
    {
        return !!this->program;
    }
    operator GLuint() const
    {
        return this->program;
    }

protected:
    GLuint program;
};