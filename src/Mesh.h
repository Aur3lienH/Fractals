#pragma once
#include "Shader.h"

class Mesh
{
public:
    Mesh(float* vertices);
    void Draw();
private:
    float* vertices;
    std::vector<Shader*> shaders;
    u_int32_t VAO = 0;
    u_int32_t vbo = 0;
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