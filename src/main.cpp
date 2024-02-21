#include <iostream>
#include "GameManager.h"
#include <string.h>

int main(int argc, char* argv[])
{
    if(argc > 1)
    {
        if(std::string(argv[1]) == "-julia")
        {
            GameManager::isJulia = true;
        }
    }
    GameManager::Init(1920, 1080, "Mandelbrot Set", true);
    return 0;
}