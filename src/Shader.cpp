#include "Shader.h"
#include <fstream>
#include <iostream>
#include <vector>

Shader::Shader(std::string path)
{
    std::ifstream file(path);
    file.seekg(0, std::ios::end); 
    int length = file.tellg();
    file.seekg(0, std::ios::beg);
    shader = new char[length];
    file.read(shader, length);
    file.close();
    CompileShader();
}

Shader::~Shader()
{
    ClearShader();
}

void Shader::CompileShader()
{
    shaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderID, 1, &shader, NULL);
    glCompileShader(shaderID);
    CheckCompileErrors(shaderID, "FRAGMENT");
}

void Shader::CheckCompileErrors(GLuint shader, std::string type)
{
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::cout << "Shader compilation error: " << &errorLog[0] << std::endl;
        glDeleteShader(shader);
    }
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if(shaderID != 0)
    {
        glDeleteShader(shaderID);
        shaderID = 0;
    }
}