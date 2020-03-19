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
        v[0] = cosf(ToRadian(i * step));
        v[1] = sinf(ToRadian(i * step));
        v[2] = 0.f;
        baseNormals[i] = v;

        v[0] = R1 * v[0] + R0;
        v[1] = R1 * v[1];
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
        c[1] = (float)i / Pieces;
        auto q = Quaternion<float>::FromAxisAngle(Vertex::YAxis, ToRadian(i * step));

        for (int j = 0; j < COUNTOF(baseVertices); j++)
        {
            c[0] = (float)j / (COUNTOF(baseVertices) - 1);

            vertices.push_back((Vertex&)q.Rotate(baseVertices[j]));
            normals.push_back((Normal&)q.Rotate(baseNormals[j]));
            texCoords.push_back(c);
        }
    }
    this->Vertices(vertices.data(), (int)vertices.size());
    this->Normals(normals.data(), (int)normals.size());
    this->TexCoords(texCoords.data(), (int)texCoords.size());

    vector<GLuint> indices;
    for (int i = 0; i < Pieces; i++)
    {
        int off0 = i * COUNTOF(baseVertices);
        int off1 = off0 + COUNTOF(baseVertices);

        for (int j = 0; j < COUNTOF(baseVertices); j++)
        {
            indices.push_back(off0 + j);
            indices.push_back(off1 + j);
            indices.push_back(off0 + j + 1);

            indices.push_back(off1 + j);
            indices.push_back(off1 + j + 1);
            indices.push_back(off0 + j + 1);
        }
    }
    this->Indices(indices.data(), (int)indices.size());
}