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
    void ProcessInput();
    void Update(float deltaTime);
    void Render();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;

    Uint64 lastFrameTime;
};