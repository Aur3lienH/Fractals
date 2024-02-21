#include "GameManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//Library for image saving 
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "imgui/imgui.h"
#include "Mesh.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"


bool GameManager::isJulia = false;
int GameManager::TargetFps = 60;
bool GameManager::rendering = true;
bool GameManager::isRunning = true;
SDL_Window* GameManager::window = nullptr;
SDL_Event GameManager::event;
int GameManager::width = 0;
int GameManager::height = 0;
const char* GameManager::title = nullptr;
bool GameManager::fullscreen = false;
std::vector<Shader*> GameManager::shaders = std::vector<Shader*>();
Vector2<double> GameManager::speeds = Vector2<double>(0.001,0.001);
Vector4<double> GameManager::Range = Vector4<double>(-1, -1, 1,1);
double GameManager::ScrollFactor = 0.1;
Vector2<double> GameManager::Complex = Vector2<double>(-0.7, 0.27015);
Vector2<double> GameManager::Rotation = Vector2<double>(0,1);
std::vector<Mesh*> GameManager::meshes = std::vector<Mesh*>();
double GameManager::Iterations = 100;
float GameManager::FloatIterations = 100;
float GameManager::LastFps = 0;

void GameManager::Init(int _width, int _height, const char* _title, bool _fullscreen)
{
    //Init Variables
    width = _width;
    height = _height;
    title = _title;
    fullscreen = _fullscreen;
    Range = Vector4<double>(-1,-1,1,height/width);


    //Init GLFW
    if(glfwInit() != GL_TRUE)
    {
        std::cout << "GLFW_Init Error: " << SDL_GetError() << std::endl;
        return;
    }
    //Init SDL
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

    if(glcontext == nullptr)
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

    //Init ImGui

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();


    ImGui_ImplSDL2_InitForOpenGL(window, glcontext);
    ImGui_ImplOpenGL3_Init("#version 330 core");




    Mesh* mesh = new Square();
    meshes.push_back(mesh);

    InitShaders();
    MainLoop();
}


void GameManager::InitShaders()
{

    Shader* shader;

    if(isJulia)
    {
        shader = new Shader("shaders/julia.glsl", GL_FRAGMENT_SHADER);
    }
    else
    {
        shader = new Shader("shaders/mandelbrot.glsl", GL_FRAGMENT_SHADER);
    }
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

    


    shaders[0]->LinkUniform2d(shaderProgram,"resolution", Vector2<double>(width,height), true);
    shaders[0]->LinkUniform4d(shaderProgram,"range", Range);
    shaders[0]->LinkUniform2d(shaderProgram,"c", Complex);
    shaders[0]->LinkUniform1d(shaderProgram,"iterations", &Iterations);



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
        if(rendering)
        {
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);
            Render();
        }
        RenderImGui(LastFps);



        

        //Timing

        SDL_GL_SwapWindow(window);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    glfwTerminate();

}


void GameManager::Update()
{
    Complex.array[0] = sin(Rotation.array[0])*Rotation.array[1];
    Complex.array[1] = cos(Rotation.array[0])*Rotation.array[1];
}

void GameManager::Render()
{
    for (int i = 0; i < shaders.size(); i++)
    {
        shaders[i]->Update();
    }
    

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GameManager::RenderImGui(float fps)
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
    ImGui::Begin("Mandelbrot");
    ImGui::Text("FPS: %f", fps);
    ImGui::SliderInt("Target FPS", &TargetFps, 1, 1000);
    if(rendering == false)
    {
        ImGui::SliderFloat("Iterations", &FloatIterations,1,200000);
    }
    else
    {
        ImGui::SliderFloat("Iterations", &FloatIterations,1,2000);
    }
    if(isJulia)
    {
        //Change the rotation of the complex number

        float rotation = static_cast<float>(Rotation.array[0]);
        ImGui::SliderFloat("Rotation", &rotation, 0, 2*3.1415);
        Rotation.array[0] = rotation;
        float magnitude = static_cast<float>(Rotation.array[1]);
        ImGui::SliderFloat("Magnitude", &magnitude, 0, 1);
        Rotation.array[1] = magnitude;



    }
    ImGui::Checkbox("Rendering", &rendering);
    if(ImGui::Button("Render"))
    {
        Render();
    }
    if(ImGui::Button("Save Image"))
    {
        SaveImage();
    }

    

    Iterations = FloatIterations;

    ImGui::End();

    ImGui::Render();

    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}







void GameManager::HandleInputs()
{
    while ( SDL_PollEvent(&event) )
    {
        int x,y;
        SDL_GetMouseState(&x,&y);
        ImGui_ImplSDL2_ProcessEvent(&event);
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
                
                Range.array[0] = Range.array[0] + (Range.array[2] - Range[0]) * x * ScrollFactor * event.wheel.y;
                Range.array[2] = Range.array[2] - (Range.array[2] - Range[0]) * (1 - x) * ScrollFactor * event.wheel.y;
                Range.array[1] = Range.array[1] + (Range.array[3] - Range.array[1]) * (1 - y) * ScrollFactor * event.wheel.y;
                Range.array[3] = Range.array[3] - (Range.array[3] - Range.array[1]) * y * ScrollFactor * event.wheel.y;
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    Rotation.array[0] -= speeds.array[0];
                    break;
                case SDLK_RIGHT:
                    Rotation.array[0] += speeds.array[0];
                    break;
                case SDLK_UP:
                    Rotation.array[1] += speeds.array[1];
                    break;
                case SDLK_DOWN:
                    Rotation.array[1] -= speeds.array[1];
                    break;
                case SDLK_w:
                    Iterations *= 1.5;
                    break;
                case SDLK_s:
                    Iterations *= 0.5;
                    break;
                case SDLK_a:
                    TargetFps /= 2;
                    break;
                case SDLK_d:
                    TargetFps *= 2;
                    break;
                case SDLK_r:
                    Render();
                    break;
                case SDLK_o:
                    rendering = !rendering;
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

void GameManager::SaveImage()
{
    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    std::string filename = std::to_string(rand() % 100000)+".png";
    stbi_write_png(filename.c_str(), width, height, 4, image.data(), width * 4);
}


