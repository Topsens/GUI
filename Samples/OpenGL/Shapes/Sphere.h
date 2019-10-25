#pragma once

#include "GLShape.h"
#include <vector>

class Sphere : public GLShape
{
public:
    Sphere(float radius = 0.5f, int recursion = 5);

private:
    void Split(float radius, int recursion, const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<Vertex>& vertices, std::vector<Normal>& normals, std::vector<Coordinate>& coords);

    int recursion;

    static const Vertex RegularTetrahedron[4];
};