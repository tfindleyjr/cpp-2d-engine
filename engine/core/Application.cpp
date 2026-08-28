#include "Application.h"

#include <iostream>

Application::Application()
    : window(nullptr),
      running(false),
      lastFrameTime(0)
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
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }
    }
}

void Application::Update(float deltaTime)
{
    (void)deltaTime;
}

void Application::Render()
{
    renderer.BeginFrame();

    renderer.SetDrawColor(
        255,
        255,
        255
    );

    renderer.FillRectangle(
        540.0f,
        300.0f,
        200.0f,
        120.0f
    );

    renderer.SetDrawColor(
        200,
        50,
        50
    );

    renderer.FillRectangle(
        100.0f,
        100.0f,
        80.0f,
        80.0f
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