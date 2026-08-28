#pragma once

#include <SDL3/SDL.h>

#include "engine/renderer/Renderer.h"
#include "engine/input/Input.h"

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
    Input input;

    bool running;

    Uint64 lastFrameTime;

    float playerX;
    float playerY;
};