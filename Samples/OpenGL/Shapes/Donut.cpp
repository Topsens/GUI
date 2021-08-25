#include "Donut.h"

const int   Pieces = 36;
const float R0 = 0.5f;
const float R1 = 0.2f;

using namespace std;

Donut::Donut()
{
    float step = 360.f / Pieces;

    Vertex baseVertices[Pieces + 1];
    Normal baseNormals[COUNTOF(baseVertices)];
    for (int i = 0; i < COUNTOF(baseVertices); i++)
    {
        Vertex v;
        v.X = cosf(ToRadian(i * step));
        v.Y = sinf(ToRadian(i * step));
        v.Z = 0.f;
        baseNormals[i] = v;

        v.X = R1 * v.X + R0;
        v.Y = R1 * v.Y;
        baseVertices[i] = v;
    }

    vector<Vertex> vertices;
    vertices.reserve((Pieces + 1) * COUNTOF(baseVertices));

    vector<Normal> normals;
    normals.reserve(vertices.size());

    vector<Coordinate> texCoords;
    texCoords.reserve(vertices.size());

    Coordinate c;
    for (int i = 0; i < Pieces + 1; i++)
    {
        c.Y = (float)i / Pieces;
        auto q = Quaternion<float>::FromAxisAngle(Vertex::YAxis, ToRadian(i * step));

        for (int j = 0; j < COUNTOF(baseVertices); j++)
        {
            c.X = (float)j / (COUNTOF(baseVertices) - 1);

            vertices.push_back((Vertex&)q.Rotate(baseVertices[j]));
            normals.push_back((Normal&)q.Rotate(baseNormals[j]));
            texCoords.push_back(c);
        }
    }
    this->Vertices(vertices.data(), (int)vertices.size());
    this->Normals(normals.data(), (int)normals.size());
    this->TexCoords(texCoords.data(), (int)texCoords.size());

    vector<Element> elements;
    for (int i = 0; i < Pieces; i++)
    {
        int off0 = i * COUNTOF(baseVertices);
        int off1 = off0 + COUNTOF(baseVertices);

        for (uint32_t j = 0; j < COUNTOF(baseVertices); j++)
        {
            elements.push_back(Element{ off0 + j, off1 + j, off0 + j + 1});
            elements.push_back(Element{ off1 + j, off1 + j + 1, off0 + j + 1 });
        }
    }
    this->Elements(elements.data(), (int)elements.size());
}