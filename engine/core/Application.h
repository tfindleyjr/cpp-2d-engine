#pragma once

#include <SDL3/SDL.h>

#include "engine/renderer/Renderer.h"

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
    Renderer renderer;

    bool running;
    Uint64 lastFrameTime;
};