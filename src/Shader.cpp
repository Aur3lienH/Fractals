#include "Shader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>


Shader::Shader(std::string path, GLuint type)
{
    UpdateFunctions = std::vector<std::function<void()>>();
    valuelinked = std::vector<GLuint>();
    this->type = type;
    std::cout << path << std::endl;
    std::ifstream file(path);
    std::string _shader = "";
    if(!file.is_open())
    {
        std::cout << "Error: File not found" << std::endl;
        return;
    }
    while(file)
    {
        char c;
        file.get(c);
        _shader += c;
        size += 1;
    }
    shader = new char[_shader.size()];
    for(int i = 0; i < _shader.size(); i++)
    {
        shader[i] = _shader[i];
    }
    std::cout << _shader << std::endl;
    file.close();
    CompileShader();
}

Shader::~Shader()
{
    ClearShader();
}

void Shader::CompileShader()
{
    shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &shader, &size);
    glCompileShader(shaderID);
    CheckCompileErrors(shaderID);
}

void Shader::CheckCompileErrors(GLuint shader)
{
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success != GL_TRUE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        //Get all the info log
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
        //Print the info log
        for(int i = 0; i < infoLog.size(); i++)
        {
            std::cout << infoLog[i];
        }

        glDeleteShader(shader);
    }
}

GLuint Shader::GetShaderID()
{
    return shaderID;
}

void Shader::ClearShader()
{
    if(shaderID != 0)
    {
        glDeleteShader(shaderID);
        shaderID = 0;
    }
}



void Shader::LinkUniform2d(GLuint programId, std::string variableName, double* x, double* y, bool onlyonce)
{
    GLint linkID = glGetUniformLocation(programId, variableName.c_str());
    if(!onlyonce)
    {
        valuelinked.push_back(linkID);
        std::function<void()> update = std::bind(glUniform2d, linkID, *x, *y);
        UpdateFunctions.push_back(update);
    }
    else
    {
        glUniform2d(linkID, *x, *y);
    }
}

void Shader::LinkUniform4d(GLuint programId, std::string variableName, double* x, double* y, double* z, double* w, bool onlyonce)
{
    GLint linkID = glGetUniformLocation(programId, variableName.c_str());
    if(!onlyonce)
    {
        valuelinked.push_back(linkID);
        std::function<void()> update = std::bind(glUniform4d, linkID, *x, *y, *z, *w);
        UpdateFunctions.push_back(update);
    }

    else
    {
        glUniform4d(linkID, *x, *y, *z, *w);
    }
}

void Shader::LinkUniform1d(GLuint programId, std::string variableName, double* x, bool onlyonce)
{
    GLint linkID = glGetUniformLocation(programId, variableName.c_str());
    if(!onlyonce)
    {
        valuelinked.push_back(linkID);
        std::function<void()> update = std::bind(glUniform1f, linkID, *x);
        UpdateFunctions.push_back(update);
    }
    else
    {
        glUniform1f(linkID, *x);
    }
}

void Shader::Update()
{
    for (int i = 0; i < UpdateFunctions.size(); i++)
    {
        UpdateFunctions[i]();
    }
}