#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Vectors.h"
#include "Mesh.h"


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
    static void RenderImGui(float fps);
    static void Update();
    static void SaveImage();


    //Variables
    static bool isRunning;
    static SDL_Window* window;
    static SDL_Event event;
    static int width;
    static int height;
    static double Iterations;
    static const char* title;
    static bool fullscreen;
    static std::vector<Shader*> shaders;
    static Vector2<double> speeds;
    static Vector2<double> Rotation;
    static Vector2<double> Complex;
    static Vector4<double> Range;
    static double ScrollFactor;
    static std::vector<Mesh*> meshes;
    static int TargetFps;
    static bool rendering;
    static float FloatIterations;
    static float LastFps;
};