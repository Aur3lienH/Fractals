#pragma once
#include "Shader.h"

class Mesh
{
public:
    Mesh();

    void Render();
private:
    std::vector<Shader*> shaders;
    GLuint VAO = 0;
    GLuint vbo = 0;
};

class Triangle : public Mesh
{
public:
    Triangle();
};

class Square : public Mesh
{
public:
    Square();
};