#include "GameManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "Mesh.h"


const int TargetFps = 60;


bool GameManager::isRunning = true;
SDL_Window* GameManager::window = nullptr;
SDL_Event GameManager::event;
int GameManager::width = 0;
int GameManager::height = 0;
const char* GameManager::title = nullptr;
bool GameManager::fullscreen = false;
std::vector<Shader*> GameManager::shaders = std::vector<Shader*>();
Vector2<double> GameManager::speeds;
Vector2<double> GameManager::RangeX;
Vector2<double> GameManager::RangeY;
double GameManager::ScrollFactor = 0.1;
Vector2<double> GameManager::Complex = Vector2<double>(-0.7, 0.27015);
std::vector<Mesh*> GameManager::meshes = std::vector<Mesh*>();

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
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    if(fullscreen)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    if(glfwInit() != GL_TRUE)
    {
        std::cout << "GLFW_Init Error: " << SDL_GetError() << std::endl;
        return;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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
    Mesh* mesh = new Square();
    meshes.push_back(mesh);
    InitShaders();
    MainLoop();
}


void GameManager::InitShaders()
{
    Shader* shader = new Shader("shaders/mandelbrot.glsl", GL_FRAGMENT_SHADER);
    shaders.push_back(shader);
    shaders.push_back(new Shader("shaders/vertex.glsl", GL_VERTEX_SHADER));


    u_int32_t shaderProgram = glCreateProgram();
    for(int i = 0; i < shaders.size(); i++)
    {
        glAttachShader(shaderProgram, shaders[i]->GetShaderID());
    }


    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    double* _width = new double(width);
    double* _height = new double(height);
    shaders[0]->LinkUniform2d(shaderProgram, "resolution", _width, _height, true);
    shaders[0]->LinkUniform4d(shaderProgram, "range", &RangeX.x, &RangeX.y, &RangeY.x, &RangeY.y);
    shaders[0]->LinkUniform2d(shaderProgram, "c", &Complex.x, &Complex.y);
    shaders[0]->LinkUniform1d(shaderProgram, "scrollFactor", &ScrollFactor);




}



void GameManager::MainLoop()
{
    int delta, leftToWait;
    int startTime, i = 0;
    while(++i && isRunning)
    {
        startTime = SDL_GetTicks(); 

        HandleInputs();
        Update();
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


void GameManager::Update()
{
    
}

void GameManager::Render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for(int i = 0; i < shaders.size(); i++)
    {
        shaders[i]->Update();
    }
    

    glDrawArrays(GL_TRIANGLES, 0, 6);

    
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
                isRunning = false;
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                int posX;
                int PosY;
                SDL_GetMouseState(&posX, &PosY);
                double x = (double)posX / (double)width;
                double y = (double)PosY / (double)height;
                
                RangeX.x = RangeX.x + (RangeX.y - RangeX.x) * x * ScrollFactor * event.wheel.y;
                RangeX.y = RangeX.y - (RangeX.y - RangeX.x) * (1 - x) * ScrollFactor * event.wheel.y;
                RangeY.x = RangeY.x + (RangeY.y - RangeY.x) * (1 - y) * ScrollFactor * event.wheel.y;
                RangeY.y = RangeY.y - (RangeY.y - RangeY.x) * y * ScrollFactor * event.wheel.y;
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if(isRunning)
                    {
                        speeds.x -= speeds.x * 0.3 + 0.0001;
                    }
                    else
                    {
                        speeds.y -= speeds.x;
                    }
                    break;
                case SDLK_RIGHT:
                    speeds.x += speeds.x * 0.3 + 0.0001;
                    break;
                case SDLK_UP:
                    speeds.y += speeds.y * 0.3 + 0.0001;
                    break;
                case SDLK_DOWN:
                    speeds.y -= speeds.y * 0.3 + 0.0001;
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