#include "Mesh.h"

Mesh::Mesh()
{

    shaders = std::vector<Shader*>();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float vertices[] = {
         -1.0f,  -1.0f, 
         1.0f, -1.0f, 
        1.0f, 1.0f,  
        1.0,1.0,
        -1.0, 1.0,
        -1.0, -1.0,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Mesh::Render()
{

}


Triangle::Triangle() : Mesh()
{
    
}

Square::Square() : Mesh()
{
    
}


