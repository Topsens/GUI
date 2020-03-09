#include "Sphere.h"
#include <cmath>

using namespace std;

#define RATIO (4.f / sqrtf(6.f))

const Vertex Sphere::RegularTetrahedron[] = {{  0.f, 1.f, 0.f },
                                             {  0.f, -1.f / 3.f, 4.f / sqrtf(18.f) },
                                             {  .5f * RATIO, -1.f / 3.f, -2.f / sqrtf(18.f)},
                                             { -.5f * RATIO, -1.f / 3.f, -2.f / sqrtf(18.f)}};

Sphere::Sphere(float radius, int recursion)
{
    if (recursion < 0)
    {
        return;
    }

    this->recursion = recursion;

    vector<Vertex> vertices;
    vector<Normal> normals;
    vector<Coordinate> coords;

    int c = COUNTOF(RegularTetrahedron);

    for (auto i = 0; i < recursion; i++)
    {
        c *= COUNTOF(RegularTetrahedron);
    }

    vertices.reserve(c);
    normals.reserve(c);

    this->Split(radius, 1, RegularTetrahedron[0], RegularTetrahedron[1], RegularTetrahedron[2], vertices, normals, coords);
    this->Split(radius, 1, RegularTetrahedron[0], RegularTetrahedron[3], RegularTetrahedron[1], vertices, normals, coords);
    this->Split(radius, 1, RegularTetrahedron[1], RegularTetrahedron[3], RegularTetrahedron[2], vertices, normals, coords);
    this->Split(radius, 1, RegularTetrahedron[0], RegularTetrahedron[2], RegularTetrahedron[3], vertices, normals, coords);

    this->Vertices(vertices.data(), (int)vertices.size());
    this->Normals(normals.data(), (int)normals.size());
    this->TexCoords(coords.data(), (int)coords.size());
}

void Sphere::Split(float radius, int recursion, const Vertex& v0, const Vertex& v1, const Vertex& v2, vector<Vertex>& vertices, vector<Normal>& normals, vector<Coordinate>& coords)
{
    if (recursion > this->recursion)
    {
        vertices.push_back(v0 * radius);
        vertices.push_back(v1 * radius);
        vertices.push_back(v2 * radius);

        normals.push_back(v0);
        normals.push_back(v1);
        normals.push_back(v2);

        coords.push_back({ (v0[0] + 1.f) / 2.f, (1.f - v0[1]) / 2.f });
        coords.push_back({ (v1[0] + 1.f) / 2.f, (1.f - v1[1]) / 2.f });
        coords.push_back({ (v2[0] + 1.f) / 2.f, (1.f - v2[1]) / 2.f });

        return;
    }

    Vertex mid[] = { (v0 + v1).Normalize(), (v1 + v2).Normalize(), (v2 + v0).Normalize() };

    recursion++;
    this->Split(radius, recursion, mid[2], v0, mid[0], vertices, normals, coords);
    this->Split(radius, recursion, mid[0], v1, mid[1], vertices, normals, coords);
    this->Split(radius, recursion, mid[1], v2, mid[2], vertices, normals, coords);
    this->Split(radius, recursion, mid[0], mid[1], mid[2], vertices, normals, coords);
}