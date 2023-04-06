#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "Shader.h"


class GameManager
{
public:
    static void Init(int _width, int _height, const char* _title, bool _fullscreen = false);
private:
    GameManager();
    static void InitShaders();
    static void MainLoop();
    static void HandleInputs();
    static void Render();

    //Variables
    static bool demoRunning;
    static SDL_Window* window;
    static SDL_Event event;
    static int width;
    static int height;
    static const char* title;
    static bool fullscreen;
    static std::vector<Shader*> shaders;
};