#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <functional>
#include "Vectors.h"

class Shader
{
public:
    Shader(std::string path, u_int32_t type);
    ~Shader();
    void ClearShader();
    GLuint GetShaderID();
    void LinkUniform1d(u_int32_t programID, std::string name, double* x, bool onlyonce = false);
    void LinkUniform2d(u_int32_t programID, std::string name, double* x, double* y, bool onlyonce = false);
    void LinkUniform4d(u_int32_t programID, std::string name, double* x, double* y, double* z, double* w, bool onlyonce = false);
    void Update();
private:
    GLuint shaderID;
    void CheckCompileErrors(GLuint shader);
    void CompileShader();
    char* shader;
    u_int32_t type;
    std::vector<u_int32_t> valuelinked;
    std::vector<std::function<void()>> UpdateFunctions;


};