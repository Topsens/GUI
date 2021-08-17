#include "Triangle.h"

Triangle::Triangle(Program& program) : program(program), cbo(GL_ARRAY_BUFFER)
{
}

void Triangle::Create()
{
    Vertex vertices[] = {{ -.5f, -.5f, 0.f }, { .5f, -.5f, 0.f }, { 0.f, .5f, 0.f }};
    this->Vertices(vertices, 3);

    Vector<float, 3> colors[] = {{ 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f }};
    this->Colors(colors, 3);

    Normal normals[] = {{ 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }};
    this->Normals(normals, 3);

    Coordinate coords[] = {{ 0.f, 1.f }, { 1.f, 1.f }, { .5f, 0.f }};
    this->TexCoords(coords, 3);
}

void Triangle::Release()
{
    GLShape::Release();
    this->cbo.Release();
}

bool Triangle::Colors(const Vector<float, 3>* colors, int count)
{
    if (!colors || count <= 0)
    {
        return false;
    }

    if (!this->cbo.Data(colors, count * sizeof(colors[0]), GL_STATIC_DRAW))
    {
        return false;
    }

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(colors[0]), 0);

    return true;
}

size_t Triangle::ApplyVertices()
{
    auto c = GLShape::ApplyVertices();
    if (c)
    {
        this->cbo.Bind();
        glEnableVertexAttribArray(1);
    }

    return c;
}

void Triangle::RevokeVertices()
{
    GLShape::RevokeVertices();

    this->cbo.Bind();
    glDisableVertexAttribArray(1);
}

void Triangle::ApplyProgram()
{
    this->program.Use();

    Matrix<float, 4> modelView, projection;
    glGetFloatv(GL_MODELVIEW_MATRIX,  modelView);
    glGetFloatv(GL_PROJECTION_MATRIX, projection);

    this->program.UniformM4f("ModelView",  modelView);
    this->program.UniformM4f("Projection", projection);
}