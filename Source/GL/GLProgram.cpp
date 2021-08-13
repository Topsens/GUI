#include "GLProgram.h"

using namespace std;

GLProgram::GLProgram() : program(0)
{
}

bool GLProgram::Create()
{
    this->Release();
    this->program = glCreateProgram();
    return !!this->program;
}

void GLProgram::Release()
{
    if (this->program)
    {
        glDeleteProgram(this->program);
        this->program = 0;
    }
}

void GLProgram::Attach(const GLShader& shader)
{
    glAttachShader(this->program, shader);
}

void GLProgram::Detach(const GLShader& shader)
{
    glDetachShader(this->program, shader);
}

bool GLProgram::Link(string& log)
{
    if (!this->Link())
    {
        GLchar  buf[4096];
        GLsizei len;
        glGetProgramInfoLog(this->program, 4096, &len, buf);
        log = buf;
        return false;
    }

    log.clear();
    return true;
}

bool GLProgram::Link()
{
    if (!this->program)
    {
        return false;
    }

    glLinkProgram(this->program);

    GLint status;
    glGetProgramiv(this->program, GL_LINK_STATUS, &status);

    return !!status;
}

void GLProgram::Use()
{
    glUseProgram(this->program);
}

void GLProgram::BindAttribLocation(GLuint index, const string& name)
{
    glBindAttribLocation(this->program, index, name.c_str());
}

GLint GLProgram::GetAttribLocation(const string& name)
{
    return glGetAttribLocation(this->program, name.c_str());
}

void GLProgram::BindFragDataLocation(GLuint index, const string& name)
{
    glBindFragDataLocation(this->program, index, name.c_str());
}

GLint GLProgram::GetFragDataLocation(const string& name)
{
    return glGetFragDataLocation(this->program, name.c_str());
}