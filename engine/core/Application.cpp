#include "Application.h"

#include <iostream>

Application::Application()
    : window(nullptr),
      running(false),
      lastFrameTime(0),
      playerX(600.0f),
      playerY(340.0f)
{
}

Application::~Application()
{
    Shutdown();
}

bool Application::Initialize()
{
    std::cout
        << "Initializing Arena Engine..."
        << std::endl;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr
            << "SDL initialization failed: "
            << SDL_GetError()
            << std::endl;

        return false;
    }

    window = SDL_CreateWindow(
        "Arena Engine",
        1280,
        720,
        0
    );

    if (!window)
    {
        std::cerr
            << "Window creation failed: "
            << SDL_GetError()
            << std::endl;

        return false;
    }

    if (!renderer.Initialize(window))
    {
        return false;
    }

    running = true;

    lastFrameTime =
        SDL_GetPerformanceCounter();

    std::cout
        << "Engine initialized successfully."
        << std::endl;

    return true;
}

void Application::Run()
{
    while (running)
    {
        Uint64 currentFrameTime =
            SDL_GetPerformanceCounter();

        double frequency =
            static_cast<double>(
                SDL_GetPerformanceFrequency()
            );

        float deltaTime =
            static_cast<float>(
                (currentFrameTime - lastFrameTime)
                / frequency
            );

        lastFrameTime = currentFrameTime;

        ProcessInput();
        Update(deltaTime);
        Render();
    }
}

void Application::ProcessInput()
{
    input.Update();

    if (input.ShouldQuit())
    {
        running = false;
    }

    if (input.IsKeyDown(SDL_SCANCODE_ESCAPE))
    {
        running = false;
    }
}

void Application::Update(float deltaTime)
{
    const float playerSpeed = 300.0f;
    const float playerSize = 50.0f;

    if (input.IsKeyDown(SDL_SCANCODE_W))
    {
        playerY -= playerSpeed * deltaTime;
    }

    if (input.IsKeyDown(SDL_SCANCODE_S))
    {
        playerY += playerSpeed * deltaTime;
    }

    if (input.IsKeyDown(SDL_SCANCODE_A))
    {
        playerX -= playerSpeed * deltaTime;
    }

    if (input.IsKeyDown(SDL_SCANCODE_D))
    {
        playerX += playerSpeed * deltaTime;
    }

    if (playerX < 0.0f)
    {
        playerX = 0.0f;
    }

    if (playerY < 0.0f)
    {
        playerY = 0.0f;
    }

    if (playerX + playerSize > 1280.0f)
    {
        playerX = 1280.0f - playerSize;
    }

    if (playerY + playerSize > 720.0f)
    {
        playerY = 720.0f - playerSize;
    }
}

void Application::Render()
{
    renderer.BeginFrame();

    // Player
    renderer.SetDrawColor(
        255,
        255,
        255
    );

    renderer.FillRectangle(
        playerX,
        playerY,
        50.0f,
        50.0f
    );

    // Temporary enemy
    renderer.SetDrawColor(
        200,
        50,
        50
    );

    renderer.FillRectangle(
        100.0f,
        100.0f,
        50.0f,
        50.0f
    );

    renderer.EndFrame();
}

void Application::Shutdown()
{
    renderer.Shutdown();

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}