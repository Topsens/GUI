#include "Square.h"

Square::Square()
{
    Vertex vUpperL = { -.5f,  .5f, 0.f };
    Vertex vUpperR = {  .5f,  .5f, 0.f };
    Vertex vLowerL = { -.5f, -.5f, 0.f };
    Vertex vLowerR = {  .5f, -.5f, 0.f };

    Vertex vertices[] = { vUpperL, vLowerL, vLowerR, vUpperL, vLowerR, vUpperR };
    this->Vertices(vertices, 6);

    Normal n = { 0.f, 0.f, 1.f };
    Normal normals[] = { n, n, n, n, n, n };
    this->Normals(normals, 6);

    Coordinate cUpperL = { 0.f, 0.f };
    Coordinate cUpperR = { 1.f, 0.f };
    Coordinate cLowerL = { 0.f, 1.f };
    Coordinate cLowerR = { 1.f, 1.f };

    Coordinate texCoords[] = { cUpperL, cLowerL, cLowerR, cUpperL, cLowerR, cUpperR };
    this->TexCoords(texCoords, 6);
}