#include "Application.h"

#include <iostream>

Application::Application()
    : window(nullptr),
      renderer(nullptr),
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
    std::cout << "Initializing Arena Engine..." << std::endl;

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

    renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer)
    {
        std::cerr
            << "Renderer creation failed: "
            << SDL_GetError()
            << std::endl;

        return false;
    }

    running = true;

    lastFrameTime = SDL_GetPerformanceCounter();

    std::cout << "Engine initialized successfully." << std::endl;

    return true;
}

void Application::Run()
{
    while (running)
    {
        Uint64 currentFrameTime = SDL_GetPerformanceCounter();

        double frequency =
            static_cast<double>(SDL_GetPerformanceFrequency());

        float deltaTime =
            static_cast<float>(
                (currentFrameTime - lastFrameTime) / frequency
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
    std::cout
        << "Delta Time: "
        << deltaTime
        << std::endl;
}

void Application::Render()
{
    SDL_SetRenderDrawColor(
        renderer,
        20,
        20,
        30,
        255
    );

    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

void Application::Shutdown()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}