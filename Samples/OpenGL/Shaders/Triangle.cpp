#include "Triangle.h"

void Triangle::Create(GLProgram* program)
{
    this->program = program;

    Vertex vertices[] = {{ -.5f, -.5f, 0.f }, { .5f, -.5f, 0.f }, { 0.f, .5f, 0.f }};
    this->Vertices(vertices, 3);

    Normal normals[] = {{ 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }};
    this->Normals(normals, 3);

    Coordinate coords[] = {{ 0.f, 1.f }, { 1.f, 1.f }, { .5f, 0.f }};
    this->TexCoords(coords, 3);
}

void Triangle::ApplyProgram()
{
    this->program->Use();

    Matrix<float, 4> modelView, projection;
    glGetFloatv(GL_MODELVIEW_MATRIX,  modelView);
    glGetFloatv(GL_PROJECTION_MATRIX, projection);

    this->program->UniformM4f("ModelView",  modelView);
    this->program->UniformM4f("Projection", projection);
    this->program->UniformV4f("Color", Vector<float, 4>{ 0.f, 1.f, 0.f, 1.f });
}