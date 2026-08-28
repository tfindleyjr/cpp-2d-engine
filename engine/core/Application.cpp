#include "Application.h"

#include <iostream>

Application::Application()
    : window(nullptr),
      running(false)
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

    running = true;

    std::cout << "Engine initialized successfully." << std::endl;

    return true;
}

void Application::Run()
{
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }
    }
}

void Application::Shutdown()
{
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}