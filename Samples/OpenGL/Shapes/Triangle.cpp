#include "Triangle.h"

Triangle::Triangle()
{
    Vertex vertices[] = {{ -.5f, -.5f, 0.f }, { .5f, -.5f, 0.f }, { 0.f, .5f, 0.f }};
    this->Vertices(vertices, 3);

    Normal normals[] = {{ 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }};
    this->Normals(normals, 3);

    Coordinate coords[] = {{ 0.f, 1.f }, { 1.f, 1.f }, { .5f, 0.f }};
    this->TexCoords(coords, 3);
}