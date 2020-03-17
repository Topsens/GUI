#include "Cylinder.h"
#include <vector>

using namespace std;

const int   Pieces = 36;
const float Radius = 0.5f;
const float Height = 1.0f;

Cylinder::Cylinder()
{
    vector<Vertex> vertices;
    vertices.reserve(Pieces * 2 + 2);

    vector<Normal> normals;
    normals.reserve(Pieces * 2 + 2);

    vector<Coordinate> texCoords;
    texCoords.reserve(Pieces * 2 + 2);

    float y = Height * .5f;
    float step = 2.f * 3.14159265358979323846f / Pieces;
    for (int i = 0; i <= Pieces; i++)
    {
        auto a = i * step;
        auto x = Radius * cosf(a);
        auto z = Radius * sinf(a);

        vertices.push_back(Vertex{ x,  y, z });
        vertices.push_back(Vertex{ x, -y, z });

        normals.push_back(Normal{ x, 0.f, z });
        normals.push_back(Normal{ x, 0.f, z });

        x = i / (float)Pieces;
        texCoords.push_back(Coordinate{ x, 0.f });
        texCoords.push_back(Coordinate{ x, 1.f });
    }

    this->Vertices(vertices.data(), (int)vertices.size());
    this->Normals(normals.data(), (int)normals.size());
    this->TexCoords(texCoords.data(), (int)texCoords.size());

    this->Mode(GL_TRIANGLE_STRIP);
}