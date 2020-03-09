#include "Circle.h"
#include <vector>

using namespace std;

Circle::Circle()
{
    int pieces = 36;
    float radius = .5f;
    float step = 2.f * 3.14159265358979323846f / pieces;

    Vertex center = { 0.f, 0.f, 0.f };

    vector<Vertex> vertices;
    vertices.reserve(pieces * 3);

    vector<Normal> normals;
    normals.reserve(pieces * 3);

    vector<Coordinate> texCoords;
    texCoords.reserve(pieces * 3);

    for (int i = 0; i < pieces; i++)
    {
        Vertex v;
        Coordinate c;

        v[0] = radius * cosf(i * step);
        v[1] = radius * sinf(i * step);
        v[2] = 0.f;
        c[0] = v[0] + .5f;
        c[1] = .5f - v[1];
        vertices.push_back(v);
        normals.push_back({ 0.f, 0.f, 1.f });
        texCoords.push_back(c);

        v[0] = radius * cosf((i + 1) * step);
        v[1] = radius * sinf((i + 1) * step);
        v[2] = 0.f;
        c[0] = v[0] + .5f;
        c[1] = .5f - v[1];
        vertices.push_back(v);
        normals.push_back({ 0.f, 0.f, 1.f });
        texCoords.push_back(c);

        vertices.push_back(center);
        normals.push_back({ 0.f, 0.f, 1.f });
        texCoords.push_back({ .5f, .5f });
    }

    this->Vertices(vertices.data(), (int)vertices.size());
    this->Normals(normals.data(), (int)normals.size());
    this->TexCoords(texCoords.data(), (int)texCoords.size());
}