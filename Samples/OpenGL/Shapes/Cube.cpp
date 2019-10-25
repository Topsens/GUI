#include "Cube.h"

Cube::Cube(float w, float h, float d)
{
    auto x = w / 2.f;
    auto y = h / 2.f;
    auto z = d / 2.f;

    Vertex vertices[] = {{ -x, -y,  z }, { x, -y,  z }, { x, y,  z }, { -x, y,  z },
                         { -x, -y, -z }, { x, -y, -z }, { x, y, -z }, { -x, y, -z }};
    GLuint vIndices[] = { 0, 1, 2, 0, 2, 3, 4, 0, 3, 4, 3, 7, 1, 5, 6, 1, 6, 2, 3, 2, 6, 3, 6, 7, 4, 5, 1, 4, 1, 0, 5, 4, 7, 5, 7, 6 };
    this->Vertices(vertices, vIndices, sizeof(vIndices) / sizeof(vIndices[0]));

    Normal normals[] = {{ 0.f, 0.f, 1.f }, { -1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, -1.f, 0.f }, { 0.f, 0.f, -1.f }};
    GLuint nIndices[] = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5 };
    this->Normals(normals, nIndices, sizeof(nIndices) / sizeof(nIndices[0]));

    Coordinate coords[] = {{ 0.f, 1.f }, { 1.f, 1.f, }, { 1.f, 0.f, }, { 0.f, 0.f }};
    GLuint cIndices[] = { 0, 1, 2, 0, 2, 3, 0, 1, 2, 0, 2, 3, 0, 1, 2, 0, 2, 3, 0, 1, 2, 0, 2, 3, 0, 1, 2, 0, 2, 3, 0, 1, 2, 0, 2, 3 };
    this->TexCoords(coords, cIndices, sizeof(cIndices) / sizeof(cIndices[0]));
}