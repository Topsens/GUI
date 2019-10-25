#include "Cylinder.h"
#include <vector>

using namespace std;

Cylinder::Cylinder(float radius, float height, int fragments)
{
    vector<Vector<float, 2>> xy;
    vector<Vector<float, 2>> uv;

    float angle = 360.0f / (float)fragments;

    for (auto i = 0; i < fragments; i++)
    {
        auto a = angle * i;

        uv.push_back({ (cosf(ToRadian(a)) + 1.f) / 2.f, (1.f - sinf(ToRadian(a))) / 2.f });
        xy.push_back({ cosf(ToRadian(a)) * radius, sinf(ToRadian(a)) * radius });
    }
    uv.push_back(uv[0]);
    xy.push_back(xy[0]);

    vector<Vertex> vertices;
    vector<Normal> normals;
    vector<Coordinate> coords;

    vertices.reserve(4 * 3 * fragments);
    normals.reserve(vertices.capacity());

    for (auto i = 0; i < fragments; i++)
    {
        auto n = i + 1;

        auto fi = i * 2 / (float)(fragments + 1);
        if (fi > 1.f)
        {
            fi -= 1.f;
        }
        auto fn = fi + 1.f / (fragments + 1);

        vertices.push_back({ xy[i][0], xy[i][1], -0.5f * height });
        vertices.push_back({ xy[i][0], xy[i][1],  0.5f * height });
        vertices.push_back({ xy[n][0], xy[n][1], -0.5f * height });
        normals.push_back({ xy[i][0], xy[i][1], 0.f });
        normals.push_back({ xy[i][0], xy[i][1], 0.f });
        normals.push_back({ xy[n][0], xy[n][1], 0.f });
        coords.push_back({ fi, 0.f });
        coords.push_back({ fi, 1.f });
        coords.push_back({ fn, 0.f });

        vertices.push_back({ xy[i][0], xy[i][1],  0.5f * height });
        vertices.push_back({ xy[n][0], xy[n][1],  0.5f * height });
        vertices.push_back({ xy[n][0], xy[n][1], -0.5f * height });
        normals.push_back({ xy[i][0], xy[i][1], 0.f });
        normals.push_back({ xy[n][0], xy[n][1], 0.f });
        normals.push_back({ xy[n][0], xy[n][1], 0.f });
        coords.push_back({ fi, 1.f });
        coords.push_back({ fn, 1.f });
        coords.push_back({ fn, 0.f });

        vertices.push_back({ 0.f, 0.f, -0.5f * height });
        vertices.push_back({ xy[i][0], xy[i][1], -0.5f * height });
        vertices.push_back({ xy[n][0], xy[n][1], -0.5f * height });
        normals.push_back({ 0.f, 0.f, -1.f });
        normals.push_back({ 0.f, 0.f, -1.f });
        normals.push_back({ 0.f, 0.f, -1.f });
        coords.push_back({ .5f, .5f });
        coords.push_back({ uv[i][0], uv[i][1] });
        coords.push_back({ uv[n][0], uv[n][1] });

        vertices.push_back({ 0.f, 0.f, 0.5f * height });
        vertices.push_back({ xy[i][0], xy[i][1], 0.5f * height });
        vertices.push_back({ xy[n][0], xy[n][1], 0.5f * height });
        normals.push_back({ 0.f, 0.f, 1.f });
        normals.push_back({ 0.f, 0.f, 1.f });
        normals.push_back({ 0.f, 0.f, 1.f });
        coords.push_back({ .5f, .5f });
        coords.push_back({ uv[i][0], uv[i][1] });
        coords.push_back({ uv[n][0], uv[n][1] });
    }

    this->Vertices(vertices.data(), (int)vertices.size());
    this->Normals(normals.data(), (int)normals.size());
    this->TexCoords(coords.data(), (int)coords.size());
}