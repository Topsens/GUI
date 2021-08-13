#include "GLShader.h"
#include <fstream>

using namespace std;

GLShader::GLShader() : shader(0)
{
}

bool GLShader::Create(GLenum type)
{
    this->Release();
    this->shader = glCreateShader(type);
    return !!this->shader;
}

void GLShader::Release()
{
    if (this->shader)
    {
        glDeleteShader(this->shader);
        this->shader = 0;
    }
}

bool GLShader::Load(const wstring& path)
{
    ifstream in(path);
    if (!in.is_open())
    {
        return false;
    }

    this->Source(string(istreambuf_iterator<char>(in), istreambuf_iterator<char>()));
    return true;
}

void GLShader::Source(const string& source)
{
    const GLchar* srcs[] = { source.c_str() };
    const GLint   lens[] = { (GLint)source.length() };
    glShaderSource(this->shader, 1, srcs, lens);
}

void GLShader::Source(const vector<string>& sources)
{
    vector<const GLchar*> srcs(sources.size());
    vector<GLint> lens(sources.size());

    for (size_t i = 0; i < sources.size(); i++)
    {
        srcs[i] = sources[i].c_str();
        lens[i] = (GLint)sources[i].length();
    }

    glShaderSource(this->shader, (GLsizei)sources.size(), srcs.data(), lens.data());
}

bool GLShader::Compile(string& log)
{
    if (!this->Compile())
    {
        GLchar  buf[4096];
        GLsizei len;
        glGetShaderInfoLog(this->shader, 4096, &len, buf);
        log = buf;
        return false;
    }

    log.clear();
    return true;
}

bool GLShader::Compile()
{
    glCompileShader(this->shader);

    GLint status;
    glGetShaderiv(this->shader, GL_COMPILE_STATUS, &status);
    return !!status;
}