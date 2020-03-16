#include "Cube.h"
#include "Square.h"

Cube::Cube()
{
    for (int i = 0; i < 6; i++)
    {
        this->AddChild(new Square());
    }

    this->children[0]->Position[2] =  .5f;  // front
    this->children[1]->Position[2] = -.5f;  // back
    this->children[2]->Position[0] = -.5f;  // left
    this->children[3]->Position[0] =  .5f;  // right
    this->children[4]->Position[1] =  .5f;  // top
    this->children[5]->Position[1] = -.5f;  // bottom

    this->children[1]->Rotation = Quaternion<float>::FromAxisAngle(Vertex::YAxis, ToRadian(180.f)).ToRotation();    // back
    this->children[2]->Rotation = Quaternion<float>::FromAxisAngle(Vertex::YAxis, ToRadian(-90.f)).ToRotation();    // left
    this->children[3]->Rotation = Quaternion<float>::FromAxisAngle(Vertex::YAxis, ToRadian( 90.f)).ToRotation();    // right
    this->children[4]->Rotation = Quaternion<float>::FromAxisAngle(Vertex::XAxis, ToRadian(-90.f)).ToRotation();    // top
    this->children[5]->Rotation = Quaternion<float>::FromAxisAngle(Vertex::XAxis, ToRadian( 90.f)).ToRotation();    // bottom
}

Cube::~Cube()
{
    for (auto child : this->children)
    {
        delete child;
    }
    this->children.clear();
}