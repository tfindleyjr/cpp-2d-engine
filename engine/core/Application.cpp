#include "Application.h"

#include <iostream>

Application::Application()
    : window(nullptr),
      player(0),
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

    player = registry.CreateEntity();

    registry.AddTransform(
        player,
        TransformComponent{
            600.0f,
            340.0f
        }
    );

    registry.AddVelocity(
        player,
        VelocityComponent{
            0.0f,
            0.0f
        }
    );

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
    TransformComponent* transform =
        registry.GetTransform(player);

    VelocityComponent* velocity =
        registry.GetVelocity(player);

    if (!transform || !velocity)
    {
        return;
    }

    const float playerSpeed = 300.0f;
    const float playerSize = 50.0f;

    velocity->x = 0.0f;
    velocity->y = 0.0f;

    if (input.IsKeyDown(SDL_SCANCODE_W))
    {
        velocity->y -= playerSpeed;
    }

    if (input.IsKeyDown(SDL_SCANCODE_S))
    {
        velocity->y += playerSpeed;
    }

    if (input.IsKeyDown(SDL_SCANCODE_A))
    {
        velocity->x -= playerSpeed;
    }

    if (input.IsKeyDown(SDL_SCANCODE_D))
    {
        velocity->x += playerSpeed;
    }

    transform->x +=
        velocity->x * deltaTime;

    transform->y +=
        velocity->y * deltaTime;

    if (transform->x < 0.0f)
    {
        transform->x = 0.0f;
    }

    if (transform->y < 0.0f)
    {
        transform->y = 0.0f;
    }

    if (
        transform->x + playerSize
        > 1280.0f
    )
    {
        transform->x =
            1280.0f - playerSize;
    }

    if (
        transform->y + playerSize
        > 720.0f
    )
    {
        transform->y =
            720.0f - playerSize;
    }
}

void Application::Render()
{
    renderer.BeginFrame();

    TransformComponent* transform =
        registry.GetTransform(player);

    if (transform)
    {
        renderer.SetDrawColor(
            255,
            255,
            255
        );

        renderer.FillRectangle(
            transform->x,
            transform->y,
            50.0f,
            50.0f
        );
    }

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