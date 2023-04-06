#include "GameManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>


const int TargetFps = 60;


bool GameManager::demoRunning = true;
SDL_Window* GameManager::window = nullptr;
SDL_Event GameManager::event;
int GameManager::width = 0;
int GameManager::height = 0;
const char* GameManager::title = nullptr;
bool GameManager::fullscreen = false;
std::vector<Shader*> GameManager::shaders;

void GameManager::Init(int _width, int _height, const char* _title, bool _fullscreen)
{
    width = _width;
    height = _height;
    title = _title;
    fullscreen = _fullscreen;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if(window == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(glContext == nullptr)
    {
        std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        return;
    }

    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
        std::cout << "GLEW_Init Error: " << glewGetErrorString(error) << std::endl;
        return;
    }

    MainLoop();
}


void GameManager::InitShaders()
{
    
}



void GameManager::MainLoop()
{
    int delta, leftToWait;
    int startTime, i = 0;
    while(++i && demoRunning)
    {
        startTime = SDL_GetTicks(); 

        HandleInputs();
        Render();

        //Timing
        delta = SDL_GetTicks() - startTime;
        leftToWait = (1000.0f / (float)TargetFps - delta);
        if(leftToWait > 0)
        {
            SDL_Delay(leftToWait);
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GameManager::Render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    


    SDL_GL_SwapWindow(window);
}



void GameManager::HandleInputs()
{
    while ( SDL_PollEvent(&event) )
    {
        int x,y;
        SDL_GetMouseState(&x,&y);

        switch( event.type )
        {
            case SDL_QUIT:
            {
                demoRunning = false;
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                std::cout << rangeLessX << " " << rangeLessY << " " << rangeMoreX << " " << rangeMoreY << std::endl;
                int posX;
                int PosY;
                SDL_GetMouseState(&posX, &PosY);
                double x = (double)posX / (double)Width;
                double y = (double)PosY / (double)Height;
                
                rangeLessX = rangeLessX + (rangeMoreX - rangeLessX) * x * ScrollFactor * event.wheel.y;
                rangeMoreX = rangeMoreX - (rangeMoreX - rangeLessX) * (1 - x) * ScrollFactor * event.wheel.y;
                rangeLessY = rangeLessY + (rangeMoreY - rangeLessY) * (1 - y) * ScrollFactor * event.wheel.y;
                rangeMoreY = rangeMoreY - (rangeMoreY - rangeLessY) * y * ScrollFactor * event.wheel.y;
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if(isRunning)
                    {
                        speed -= speed * 0.3 + 0.0001;
                    }
                    else
                    {
                        angle -= speed;
                    }
                    break;
                case SDLK_RIGHT:
                    speed += speed * 0.3 + 0.0001;
                    break;
                case SDLK_UP:
                    speedRadius += speedRadius * 0.3 + 0.0001;
                    break;
                case SDLK_DOWN:
                    speedRadius -= speedRadius * 0.3 + 0.0001;
                    break;
                case SDLK_SPACE:
                {
                    isRunning = !isRunning;
                    break;
                }
                default:
                    break;
                }
                break;
            }
        }
    }
}