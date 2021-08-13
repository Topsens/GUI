#pragma once

#include "GLCommon.h"
#include <string>
#include <vector>

class GLShader
{
public:
    GLShader();

    virtual bool Create(GLenum type);
    virtual void Release();
    virtual bool Load(const std::wstring& path);

    void Source(const std::string& source);
    void Source(const std::vector<std::string>& sources);
    bool Compile(std::string& log);
    bool Compile();

    operator bool() const
    {
        return !!this->shader;
    }
    operator GLuint() const
    {
        return this->shader;
    }

protected:
    GLuint shader;
};