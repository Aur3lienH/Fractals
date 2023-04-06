#include "Mesh.h"

Mesh::Mesh(float* vertices)
{
    this->vertices = vertices;

    shaders = std::vector<Shader*>();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Mesh::Draw()
{
    
}


Triangle::Triangle() : Mesh(new float[6]{-1.0, -1.0, 1.0, -1.0, 0.0, 1.0})
{
    
}

Square::Square() : Mesh(new float[12]{
    -1.0f,  -1.0f, 
         1.0f, -1.0f, 
        1.0f, 1.0f,  
        1.0,1.0,
        -1.0, 1.0,
        -1.0, -1.0,})
{
    
}


