#pragma once

#include <SDL3/SDL.h>

class Application
{
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    SDL_Window* window;
    bool running;
};