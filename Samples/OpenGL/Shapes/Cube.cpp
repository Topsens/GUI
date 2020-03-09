#include "Cube.h"

Cube::Cube()
{
    Vertex ful = { -.5f,  .5f, .5f };
    Vertex fur = {  .5f,  .5f, .5f };
    Vertex fll = { -.5f, -.5f, .5f };
    Vertex flr = {  .5f, -.5f, .5f };
    Vertex bul = { ful[0], ful[1], -.5f };
    Vertex bur = { fur[0], fur[1], -.5f };
    Vertex bll = { fll[0], fll[1], -.5f };
    Vertex blr = { flr[0], flr[1], -.5f };

    Vertex vertices[][3] = {{ ful, fll, flr }, { ful, flr, fur },   // front
                            { bur, blr, bll }, { bur, bll, bul },   // back
                            { bul, bll, fll }, { bul, fll, ful },   // left
                            { fur, flr, blr }, { fur, blr, bur },   // right
                            { bul, ful, fur }, { bul, fur, bur },   // top
                            { fll, bll, blr }, { fll, blr, flr }};  // bottom
    this->Vertices(vertices[0], sizeof(vertices) / sizeof(vertices[0][0]));

    Vertex frt = {  0.f,  0.f,  1.f };
    Vertex bck = {  0.f,  0.f, -1.f };
    Vertex lft = { -1.f,  0.f,  0.f };
    Vertex rgt = {  1.f,  0.f,  0.f };
    Vertex top = {  0.f,  1.f,  0.f };
    Vertex btm = {  0.f, -1.f,  0.f };

    Normal normals[][3] = {{ frt, frt, frt }, { frt, frt, frt},
                           { bck, bck, bck }, { bck, bck, bck },
                           { lft, lft, lft }, { lft, lft, lft },
                           { rgt, rgt, rgt }, { rgt, rgt, rgt },
                           { top, top, top }, { top, top, top },
                           { btm, btm, btm }, { btm, btm, btm }};
    this->Normals(normals[0], sizeof(normals) / sizeof(normals[0][0]));

    Coordinate ul = { 0.f, 0.f };
    Coordinate ur = { 1.f, 0.f };
    Coordinate ll = { 0.f, 1.f };
    Coordinate lr = { 1.f, 1.f };

    Coordinate texCoords[][3] = {{ ul, ll, lr }, { ul, lr, ur }, // front
                                 { ul, ll, lr }, { ul, lr, ur }, // back
                                 { ul, ll, lr }, { ul, lr, ur }, // left
                                 { ul, ll, lr }, { ul, lr, ur }, // right
                                 { ul, ll, lr }, { ul, lr, ur }, // top
                                 { ul, ll, lr }, { ul, lr, ur }};// bottom
    this->TexCoords(texCoords[0], sizeof(texCoords) / sizeof(texCoords[0][0]));
}