#include "Application.h"

#include "engine/physics/Physics.h"

#include <iostream>

Application::Application()
    : window(nullptr),
      player(0),
      obstacle(0),
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

    if (
        !textureManager.LoadTexture(
            renderer.GetSDLRenderer(),
            "player",
            "assets/player.bmp"
        )
    )
    {
        return false;
    }

    // -------------------------
    // Player Entity
    // -------------------------

    player =
        registry.CreateEntity();

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

    registry.AddCollider(
        player,
        ColliderComponent{
            50.0f,
            50.0f
        }
    );

    registry.AddSprite(
        player,
        SpriteComponent{
            "player",
            64.0f,
            64.0f,
            32,
            32,
            0
        }
    );

    registry.AddAnimation(
        player,
        AnimationComponent{
            4,
            0.15f,
            0.0f,
            true
        }
    );

    // -------------------------
    // Obstacle Entity
    // -------------------------

    obstacle =
        registry.CreateEntity();

    registry.AddTransform(
        obstacle,
        TransformComponent{
            500.0f,
            200.0f
        }
    );

    registry.AddCollider(
        obstacle,
        ColliderComponent{
            300.0f,
            50.0f
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
                (
                    currentFrameTime -
                    lastFrameTime
                )
                / frequency
            );

        lastFrameTime =
            currentFrameTime;

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

    if (
        input.IsKeyDown(
            SDL_SCANCODE_ESCAPE
        )
    )
    {
        running = false;
    }
}

void Application::Update(
    float deltaTime
)
{
    TransformComponent* playerTransform =
        registry.GetTransform(player);

    VelocityComponent* playerVelocity =
        registry.GetVelocity(player);

    ColliderComponent* playerCollider =
        registry.GetCollider(player);

    TransformComponent* obstacleTransform =
        registry.GetTransform(obstacle);

    ColliderComponent* obstacleCollider =
        registry.GetCollider(obstacle);

    if (
        !playerTransform ||
        !playerVelocity ||
        !playerCollider
    )
    {
        return;
    }

    const float previousX =
        playerTransform->x;

    const float previousY =
        playerTransform->y;

    const float playerSpeed =
        300.0f;

    playerVelocity->x = 0.0f;
    playerVelocity->y = 0.0f;

    // -------------------------
    // Input
    // -------------------------

    if (
        input.IsKeyDown(
            SDL_SCANCODE_W
        )
    )
    {
        playerVelocity->y -=
            playerSpeed;
    }

    if (
        input.IsKeyDown(
            SDL_SCANCODE_S
        )
    )
    {
        playerVelocity->y +=
            playerSpeed;
    }

    if (
        input.IsKeyDown(
            SDL_SCANCODE_A
        )
    )
    {
        playerVelocity->x -=
            playerSpeed;
    }

    if (
        input.IsKeyDown(
            SDL_SCANCODE_D
        )
    )
    {
        playerVelocity->x +=
            playerSpeed;
    }

    // -------------------------
    // Diagonal normalization
    // -------------------------

    if (
        playerVelocity->x != 0.0f &&
        playerVelocity->y != 0.0f
    )
    {
        constexpr float diagonalScale =
            0.70710678f;

        playerVelocity->x *=
            diagonalScale;

        playerVelocity->y *=
            diagonalScale;
    }

    // -------------------------
    // Movement
    // -------------------------

    playerTransform->x +=
        playerVelocity->x *
        deltaTime;

    playerTransform->y +=
        playerVelocity->y *
        deltaTime;

    // -------------------------
    // Screen boundaries
    // -------------------------

    if (playerTransform->x < 0.0f)
    {
        playerTransform->x =
            0.0f;
    }

    if (playerTransform->y < 0.0f)
    {
        playerTransform->y =
            0.0f;
    }

    if (
        playerTransform->x +
        playerCollider->width >
        1280.0f
    )
    {
        playerTransform->x =
            1280.0f -
            playerCollider->width;
    }

    if (
        playerTransform->y +
        playerCollider->height >
        720.0f
    )
    {
        playerTransform->y =
            720.0f -
            playerCollider->height;
    }

    // -------------------------
    // Collision
    // -------------------------

    if (
        obstacleTransform &&
        obstacleCollider
    )
    {
        const bool collided =
            Physics::CheckCollision(
                *playerTransform,
                *playerCollider,
                *obstacleTransform,
                *obstacleCollider
            );

        if (collided)
        {
            playerTransform->x =
                previousX;

            playerTransform->y =
                previousY;
        }
    }

    // -------------------------
    // Animation
    // -------------------------

    SpriteComponent* sprite =
        registry.GetSprite(player);

    AnimationComponent* animation =
        registry.GetAnimation(player);

    if (sprite && animation)
    {
        animation->elapsedTime +=
            deltaTime;

        if (
            animation->elapsedTime >=
            animation->frameDuration
        )
        {
            animation->elapsedTime -=
                animation->frameDuration;

            sprite->currentFrame++;

            if (
                sprite->currentFrame >=
                animation->frameCount
            )
            {
                if (animation->looping)
                {
                    sprite->currentFrame =
                        0;
                }
                else
                {
                    sprite->currentFrame =
                        animation->frameCount -
                        1;
                }
            }
        }
    }
}

void Application::Render()
{
    renderer.BeginFrame();

    // -------------------------
    // Player sprite
    // -------------------------

    TransformComponent* playerTransform =
        registry.GetTransform(player);

    SpriteComponent* playerSprite =
        registry.GetSprite(player);

    if (
        playerTransform &&
        playerSprite
    )
    {
        SDL_Texture* playerTexture =
            textureManager.GetTexture(
                playerSprite->textureId
            );

        SDL_FRect source{
            static_cast<float>(
                playerSprite->currentFrame *
                playerSprite->frameWidth
            ),
            0.0f,
            static_cast<float>(
                playerSprite->frameWidth
            ),
            static_cast<float>(
                playerSprite->frameHeight
            )
        };

        SDL_FRect destination{
            playerTransform->x,
            playerTransform->y,
            playerSprite->width,
            playerSprite->height
        };

        renderer.DrawTexture(
            playerTexture,
            &source,
            &destination
        );
    }

    // -------------------------
    // Obstacle
    // -------------------------

    TransformComponent* obstacleTransform =
        registry.GetTransform(obstacle);

    ColliderComponent* obstacleCollider =
        registry.GetCollider(obstacle);

    if (
        obstacleTransform &&
        obstacleCollider
    )
    {
        renderer.SetDrawColor(
            80,
            80,
            100
        );

        renderer.FillRectangle(
            obstacleTransform->x,
            obstacleTransform->y,
            obstacleCollider->width,
            obstacleCollider->height
        );
    }

    renderer.EndFrame();
}

void Application::Shutdown()
{
    textureManager.Shutdown();

    renderer.Shutdown();

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}