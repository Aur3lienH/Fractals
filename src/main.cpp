#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#define GLSL(src) "#version 430 core\n" #src

const int Width = 1920;
const int Height = 1080;
const int TargetFps = 60;

const double ScrollFactor = 0.1f;


SDL_Event event;
bool demoRunning = true;
double rangeLessX = -2;
double rangeLessY = -1;
double rangeMoreX = 1;
double rangeMoreY = 1;

double ReelPart = -0.8406;
double ImaginaryPart = 0.1242;

double Radius = 0;
double angle = 0;
double speed = 0.0005;
double speedRadius = 0.0001;

bool isRunning = true;

void HandleInputs()
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


int main(int argc, char** argv)
{
    // -------------------------------- INIT ------------------------------- //

    // Init GLFW
    glutInit(&argc, argv);
    if (glfwInit() != GL_TRUE) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    

    SDL_Window* window = SDL_CreateWindow("Mandelbrot",100,100,Width, Height, SDL_WINDOW_OPENGL);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    // Init GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    


    // ----------------------------- RESOURCES ----------------------------- //

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
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

    // Create and compile the vertex shader
    const char* vertexSource = GLSL(
        in vec2 position;
        
        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
        }
    );

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    /*
    const char* fragmentSource = GLSL(
        out vec4 outColor;
        uniform dvec2 resolution;
        uniform dvec4 range;
        void main() {
            dvec2 coor = gl_FragCoord.xy / resolution.xy;
            dvec2 c = dvec2(range.x + coor.x * (range.z - range.x), range.y + coor.y * (range.w - range.y));
            dvec2 z = dvec2(0.0, 0.0);
            double iterations = 50.0;
            for(int i = 0; i < iterations; i++)
            {
                z = dvec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
                double r = z.x * z.x + z.y * z.y;
                if(r > 4)
                {
                    double value = (iterations-i)/iterations;
                    outColor = vec4(0.0, 0.1, 1-value, 1.0);
                    return;
                }
            }
            double r = z.x * z.x + z.y * z.y;
            outColor = vec4(0.0,0.0,0.0, 1.0);

        }
    );
    */
   const char* fragmentSource = GLSL(
        out vec4 outColor;
        uniform dvec2 resolution;
        uniform dvec4 range;
        uniform dvec2 c;
        void main() {
            dvec2 coor = gl_FragCoord.xy / resolution.xy;
            dvec2 z = dvec2(range.x + coor.x * (range.z - range.x), range.y + coor.y * (range.w - range.y));
            double iterations = 50.0;
            for(int i = 0; i < iterations; i++)
            {
                z = dvec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
                double r = z.x * z.x + z.y * z.y;
                if(r > 4)
                {
                    double value = i/iterations;
                    if(value > 0.666666)
                    {
                        value = (value - 0.666666) * 3;
                        outColor = vec4(1-value,0.0, 0.0 , 1.0);
                    }
                    else if(value > 0.3333333)
                    {
                        value = (value - 0.3333333) * 3;
                        outColor = vec4(0.0,1-value, 0.0 , 1.0);
                    }
                    else
                    {
                        value = value * 3;
                        outColor = vec4(0.0,0.0, 1-value , 1.0);
                    }
                    return;
                }
            }
            double r = z.x * z.x + z.y * z.y;
            outColor = vec4(0.0,0.0,0.0, 1.0);

        }
    );



    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);



    int isCompiled = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);
        std::cout << "Error: " << errorLog.data() << std::endl;
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fragmentShader); // Don't leak the shader.
        return -1;
    }


    

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // ---------------------------- UNIFORMS ------------------------------ //

    // Get the location of the resolution uniform
    GLint uniResolution = glGetUniformLocation(shaderProgram, "resolution");
    // Pass the resolution to the shader
    glUniform2d(uniResolution, Width, Height);
    uniResolution = glGetUniformLocation(shaderProgram, "range");

    GLint c = glGetUniformLocation(shaderProgram, "c");
    glUniform2d(c, ReelPart, ImaginaryPart);
    


    // ---------------------------- RENDERING ------------------------------ //

    int delta, leftToWait;
    int startTime, i = 0;

    while(demoRunning)
    {
        startTime = SDL_GetTicks(); 
		// Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw a triangle from the 3 vertices
        glUniform4d(uniResolution, rangeLessX, rangeLessY, rangeMoreX, rangeMoreY);
        glUniform2d(c,ReelPart,ImaginaryPart);
        ReelPart = cos(angle) * Radius;
        ImaginaryPart = sin(angle) * Radius;
        if(isRunning)
        {
            angle += speed;
            Radius += speedRadius;
        }

        glDrawArrays(GL_TRIANGLES, 0, 6);



        // Swap buffers and poll window events
        SDL_GL_SwapWindow(window);

        HandleInputs();
        
         

        //Time management
        delta = SDL_GetTicks() - startTime;
        leftToWait = (1000.0f / (float)TargetFps - delta);
        if(leftToWait > 0)
        {
            SDL_Delay(leftToWait);
        }
    }

    // ---------------------------- CLEARING ------------------------------ //

    // Delete allocated resources
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    // ---------------------------- TERMINATE ----------------------------- //

    // Terminate GLFW
    glfwTerminate();

    return 0;
}