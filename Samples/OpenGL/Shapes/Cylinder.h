#pragma once

#include "GLShape.h"

class Cylinder : public GLShape
{
public:
    Cylinder(float radius = .5f, float height = 1.f, int fragments = 120);
};