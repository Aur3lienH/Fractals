#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

class Shader
{
public:
    Shader(std::string path);
    ~Shader();
    void CompileShader();
    void UseShader();
    void ClearShader();
    GLuint GetShaderID();
private:
    GLuint shaderID;
    void CheckCompileErrors(GLuint shader, std::string type);
    char* shader;

};