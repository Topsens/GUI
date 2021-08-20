#pragma once

#include "GLProgram.h"

class Program : public GLProgram
{
public:
    bool Create(const std::wstring& vshader, const std::wstring& fshader);
    void Release() override;

private:
    GLShader vshader;
    GLShader fshader;
};