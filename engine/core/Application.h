#pragma once

#include <SDL3/SDL.h>

#include "engine/renderer/Renderer.h"
#include "engine/input/Input.h"
#include "engine/ecs/Registry.h"
#include "engine/resources/TextureManager.h"

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

    TextureManager textureManager;

    Registry registry;

    Entity player;
    Entity obstacle;

    bool running;

    Uint64 lastFrameTime;
};