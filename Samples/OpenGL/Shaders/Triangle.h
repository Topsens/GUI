#pragma once

#include "GLShape.h"
#include "Program.h"

class Triangle : public GLShape
{
public:
    Triangle(Program& program);

    void Create();
    void Release() override;
    
    bool Colors(const Vector<float, 3>* colors, int count);

protected:
    size_t ApplyVertices() override;
    void   RevokeVertices() override;
    void   ApplyProgram() override;

    Program& program;
    GLBuffer cbo;
};