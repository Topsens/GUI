#include "Circle.h"
#include <vector>

using namespace std;

const int   Pieces = 36;
const float Radius = 0.5f;

Circle::Circle()
{
    vector<Vertex> vertices;
    vertices.reserve(Pieces + 2);

    vector<Normal> normals;
    normals.reserve(vertices.size());

    vector<Coordinate> texCoords;
    texCoords.reserve(vertices.size());

    Vertex center = { 0.f, 0.f, 0.f };
    Normal normal = { 0.f, 0.f, 1.f };
    vertices.push_back(center);
    normals.push_back(normal);
    texCoords.push_back({ .5f, .5f });

    Vertex v = { 0.f, 0.f, 0.f };
    Coordinate c;

    float step = 2.f * 3.14159265358979323846f / Pieces;
    for (int i = 0; i <= Pieces; i++)
    {
        v.X = Radius * cosf(i * step);
        v.Y = Radius * sinf(i * step);
        c.X = v.X + .5f;
        c.Y = .5f - v.Y;

        vertices.push_back(v);
        texCoords.push_back(c);
        normals.push_back(normal);
    }

    this->Vertices(vertices.data(), (int)vertices.size());
    this->Normals(normals.data(), (int)normals.size());
    this->TexCoords(texCoords.data(), (int)texCoords.size());

    this->Mode(GL_TRIANGLE_FAN);
}