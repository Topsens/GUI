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

void GLProgram::Use()
{
    glUseProgram(this->program);
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

bool GLProgram::UniformV3f(const string& name, const Vector<float, 3>& value)
{
    if (!this->program)
    {
        return false;
    }

    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    if (program != this->program)
    {
        return false;
    }

    auto loc = glGetUniformLocation(this->program, name.c_str());
    if (loc < 0)
    {
        return false;
    }

    glUniform3f(loc, value[0], value[1], value[2]);
    return true;
}

bool GLProgram::UniformV4f(const string& name, const Vector<float, 4>& value)
{
    if (!this->program)
    {
        return false;
    }

    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    if (program != this->program)
    {
        return false;
    }

    auto loc = glGetUniformLocation(this->program, name.c_str());
    if (loc < 0)
    {
        return false;
    }

    glUniform4f(loc, value[0], value[1], value[2], value[3]);
    return true;
}

bool GLProgram::UniformM4f(const string& name, const Matrix<float, 4>& value)
{
    if (!this->program)
    {
        return false;
    }

    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    if (program != this->program)
    {
        return false;
    }

    auto loc = glGetUniformLocation(this->program, name.c_str());
    if (loc < 0)
    {
        return false;
    }

    glUniformMatrix4fv(loc, 1, GL_FALSE, value);
    return true;
}