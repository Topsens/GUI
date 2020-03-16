#include "Square.h"

Square::Square()
{
    Vertex vUpperL = { -.5f,  .5f, 0.f };
    Vertex vUpperR = {  .5f,  .5f, 0.f };
    Vertex vLowerL = { -.5f, -.5f, 0.f };
    Vertex vLowerR = {  .5f, -.5f, 0.f };
    Vertex vertices[] = { vUpperL, vUpperR, vLowerL, vLowerR };
    this->Vertices(vertices, sizeof(vertices) / sizeof(vertices[0]));

    Normal n = { 0.f, 0.f, 1.f };
    Normal normals[] = { n, n, n, n };
    this->Normals(normals, sizeof(normals) / sizeof(normals[0]));

    Coordinate cUpperL = { 0.f, 0.f };
    Coordinate cUpperR = { 1.f, 0.f };
    Coordinate cLowerL = { 0.f, 1.f };
    Coordinate cLowerR = { 1.f, 1.f };
    Coordinate texCoords[] = { cUpperL, cUpperR, cLowerL, cLowerR };
    this->TexCoords(texCoords, sizeof(texCoords) / sizeof(texCoords[0]));

    this->Mode(GL_TRIANGLE_STRIP);
}