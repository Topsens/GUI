#pragma once

#include "GLShape.h"
#include "GLProgram.h"

class Triangle : public GLShape
{
public:
    void Create(GLProgram* program);

protected:
    void ApplyProgram() override;

    GLProgram* program;
};