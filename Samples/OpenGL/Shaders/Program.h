#pragma once

#include "GLProgram.h"

class Program : public GLProgram
{
public:
    bool Create() override;
    void Release() override;

private:
    GLShader vshader;
    GLShader fshader;
};