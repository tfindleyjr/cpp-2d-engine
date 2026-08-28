#include "Application.h"

#include "engine/physics/Physics.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_set>

Application::Application()
    : window(nullptr),
      camera(ScreenWidth, ScreenHeight),
      player(0),
      obstacle(0),
      running(false),
      lastFrameTime(0),
      shootCooldown(0.0f)
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
        static_cast<int>(ScreenWidth),
        static_cast<int>(ScreenHeight),
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

    if (!textureManager.LoadTexture(
            renderer.GetSDLRenderer(),
            "player",
            "assets/player.bmp"
        ))
    {
        std::cout
            << "Player sprite was not found. Using the built-in rectangle fallback."
            << std::endl;
    }

    camera.SetWorldBounds(WorldWidth, WorldHeight);

    // Player
    player = registry.CreateEntity();

    registry.AddTransform(
        player,
        TransformComponent{1175.0f, 775.0f}
    );

    registry.AddVelocity(
        player,
        VelocityComponent{0.0f, 0.0f}
    );

    registry.AddCollider(
        player,
        ColliderComponent{50.0f, 50.0f}
    );

    registry.AddHealth(
        player,
        HealthComponent{5, 5}
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

    // World obstacle
    obstacle = registry.CreateEntity();

    registry.AddTransform(
        obstacle,
        TransformComponent{950.0f, 620.0f}
    );

    registry.AddCollider(
        obstacle,
        ColliderComponent{500.0f, 70.0f}
    );

    // Enemy starting positions
    SpawnEnemy(300.0f, 250.0f);
    SpawnEnemy(2050.0f, 250.0f);
    SpawnEnemy(300.0f, 1250.0f);
    SpawnEnemy(2050.0f, 1250.0f);
    SpawnEnemy(1150.0f, 250.0f);
    SpawnEnemy(1150.0f, 1320.0f);

    TransformComponent* playerTransform =
        registry.GetTransform(player);
    ColliderComponent* playerCollider =
        registry.GetCollider(player);

    if (playerTransform && playerCollider)
    {
        camera.Follow(
            playerTransform->x,
            playerTransform->y,
            playerCollider->width,
            playerCollider->height
        );
    }

    running = true;
    lastFrameTime = SDL_GetPerformanceCounter();

    std::cout
        << "Engine initialized successfully."
        << std::endl;
    std::cout
        << "Controls: WASD move, mouse aim, left click shoot, ESC quit."
        << std::endl;

    return true;
}

void Application::Run()
{
    while (running)
    {
        const Uint64 currentFrameTime =
            SDL_GetPerformanceCounter();

        const double frequency =
            static_cast<double>(SDL_GetPerformanceFrequency());

        float deltaTime = static_cast<float>(
            (currentFrameTime - lastFrameTime) / frequency
        );

        lastFrameTime = currentFrameTime;

        // Prevent giant physics jumps after a breakpoint or window stall.
        deltaTime = std::min(deltaTime, 0.05f);

        ProcessInput();
        Update(deltaTime);
        Render();
    }
}

void Application::ProcessInput()
{
    input.Update();

    if (input.ShouldQuit() || input.IsKeyDown(SDL_SCANCODE_ESCAPE))
    {
        running = false;
    }
}

void Application::Update(float deltaTime)
{
    if (shootCooldown > 0.0f)
    {
        shootCooldown -= deltaTime;
    }

    UpdatePlayer(deltaTime);

    if (!running)
    {
        return;
    }

    UpdateEnemies(deltaTime);
    UpdateBullets(deltaTime);
    UpdatePlayerAnimation(deltaTime);

    TransformComponent* playerTransform =
        registry.GetTransform(player);
    ColliderComponent* playerCollider =
        registry.GetCollider(player);

    if (playerTransform && playerCollider)
    {
        camera.Follow(
            playerTransform->x,
            playerTransform->y,
            playerCollider->width,
            playerCollider->height
        );
    }
}

void Application::UpdatePlayer(float deltaTime)
{
    TransformComponent* transform =
        registry.GetTransform(player);
    VelocityComponent* velocity =
        registry.GetVelocity(player);
    ColliderComponent* collider =
        registry.GetCollider(player);
    HealthComponent* health =
        registry.GetHealth(player);

    if (!transform || !velocity || !collider || !health)
    {
        return;
    }

    if (health->current <= 0)
    {
        std::cout << "Game Over" << std::endl;
        running = false;
        return;
    }

    const float previousX = transform->x;
    const float previousY = transform->y;
    constexpr float playerSpeed = 300.0f;

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

    if (velocity->x != 0.0f && velocity->y != 0.0f)
    {
        constexpr float diagonalScale = 0.70710678f;
        velocity->x *= diagonalScale;
        velocity->y *= diagonalScale;
    }

    transform->x += velocity->x * deltaTime;
    transform->y += velocity->y * deltaTime;

    transform->x = std::clamp(
        transform->x,
        0.0f,
        WorldWidth - collider->width
    );

    transform->y = std::clamp(
        transform->y,
        0.0f,
        WorldHeight - collider->height
    );

    TransformComponent* obstacleTransform =
        registry.GetTransform(obstacle);
    ColliderComponent* obstacleCollider =
        registry.GetCollider(obstacle);

    if (
        obstacleTransform &&
        obstacleCollider &&
        Physics::CheckCollision(
            *transform,
            *collider,
            *obstacleTransform,
            *obstacleCollider
        )
    )
    {
        transform->x = previousX;
        transform->y = previousY;
    }

    camera.Follow(
        transform->x,
        transform->y,
        collider->width,
        collider->height
    );

    if (
        input.IsMouseButtonDown(SDL_BUTTON_LMASK) &&
        shootCooldown <= 0.0f
    )
    {
        SpawnBullet();
        shootCooldown = 0.16f;
    }
}

void Application::SpawnBullet()
{
    TransformComponent* playerTransform =
        registry.GetTransform(player);
    ColliderComponent* playerCollider =
        registry.GetCollider(player);

    if (!playerTransform || !playerCollider)
    {
        return;
    }

    float mouseX = 0.0f;
    float mouseY = 0.0f;
    input.GetMousePosition(mouseX, mouseY);

    const float mouseWorldX = mouseX + camera.GetX();
    const float mouseWorldY = mouseY + camera.GetY();

    const float playerCenterX =
        playerTransform->x + playerCollider->width * 0.5f;
    const float playerCenterY =
        playerTransform->y + playerCollider->height * 0.5f;

    float directionX = mouseWorldX - playerCenterX;
    float directionY = mouseWorldY - playerCenterY;

    const float length = std::sqrt(
        directionX * directionX + directionY * directionY
    );

    if (length < 0.001f)
    {
        return;
    }

    directionX /= length;
    directionY /= length;

    constexpr float bulletSize = 10.0f;
    constexpr float bulletSpeed = 750.0f;

    const Entity bullet = registry.CreateEntity();

    registry.AddTransform(
        bullet,
        TransformComponent{
            playerCenterX - bulletSize * 0.5f,
            playerCenterY - bulletSize * 0.5f
        }
    );

    registry.AddVelocity(
        bullet,
        VelocityComponent{
            directionX * bulletSpeed,
            directionY * bulletSpeed
        }
    );

    registry.AddCollider(
        bullet,
        ColliderComponent{bulletSize, bulletSize}
    );

    registry.AddProjectile(
        bullet,
        ProjectileComponent{2.2f, 1}
    );

    bullets.push_back(bullet);
}

void Application::UpdateBullets(float deltaTime)
{
    std::unordered_set<Entity> deadBullets;
    std::unordered_set<Entity> deadEnemies;

    TransformComponent* obstacleTransform =
        registry.GetTransform(obstacle);
    ColliderComponent* obstacleCollider =
        registry.GetCollider(obstacle);

    for (Entity bullet : bullets)
    {
        TransformComponent* transform =
            registry.GetTransform(bullet);
        VelocityComponent* velocity =
            registry.GetVelocity(bullet);
        ColliderComponent* collider =
            registry.GetCollider(bullet);
        ProjectileComponent* projectile =
            registry.GetProjectile(bullet);

        if (!transform || !velocity || !collider || !projectile)
        {
            deadBullets.insert(bullet);
            continue;
        }

        transform->x += velocity->x * deltaTime;
        transform->y += velocity->y * deltaTime;
        projectile->lifetime -= deltaTime;

        const bool outsideWorld =
            transform->x + collider->width < 0.0f ||
            transform->y + collider->height < 0.0f ||
            transform->x > WorldWidth ||
            transform->y > WorldHeight;

        const bool hitObstacle =
            obstacleTransform &&
            obstacleCollider &&
            Physics::CheckCollision(
                *transform,
                *collider,
                *obstacleTransform,
                *obstacleCollider
            );

        if (projectile->lifetime <= 0.0f || outsideWorld || hitObstacle)
        {
            deadBullets.insert(bullet);
            continue;
        }

        for (Entity enemy : enemies)
        {
            if (deadEnemies.contains(enemy))
            {
                continue;
            }

            TransformComponent* enemyTransform =
                registry.GetTransform(enemy);
            ColliderComponent* enemyCollider =
                registry.GetCollider(enemy);
            HealthComponent* enemyHealth =
                registry.GetHealth(enemy);

            if (!enemyTransform || !enemyCollider || !enemyHealth)
            {
                continue;
            }

            if (Physics::CheckCollision(
                    *transform,
                    *collider,
                    *enemyTransform,
                    *enemyCollider
                ))
            {
                enemyHealth->current -= projectile->damage;
                deadBullets.insert(bullet);

                if (enemyHealth->current <= 0)
                {
                    deadEnemies.insert(enemy);
                }

                break;
            }
        }
    }

    for (Entity bullet : deadBullets)
    {
        registry.DestroyEntity(bullet);
    }

    for (Entity enemy : deadEnemies)
    {
        registry.DestroyEntity(enemy);
    }

    std::erase_if(
        bullets,
        [&deadBullets](Entity entity)
        {
            return deadBullets.contains(entity);
        }
    );

    std::erase_if(
        enemies,
        [&deadEnemies](Entity entity)
        {
            return deadEnemies.contains(entity);
        }
    );
}

void Application::SpawnEnemy(float x, float y)
{
    const Entity enemy = registry.CreateEntity();

    registry.AddTransform(
        enemy,
        TransformComponent{x, y}
    );

    registry.AddVelocity(
        enemy,
        VelocityComponent{0.0f, 0.0f}
    );

    registry.AddCollider(
        enemy,
        ColliderComponent{44.0f, 44.0f}
    );

    registry.AddHealth(
        enemy,
        HealthComponent{3, 3}
    );

    registry.AddEnemy(
        enemy,
        EnemyComponent{
            120.0f,
            1,
            0.75f,
            0.0f
        }
    );

    enemies.push_back(enemy);
}

void Application::UpdateEnemies(float deltaTime)
{
    TransformComponent* playerTransform =
        registry.GetTransform(player);
    ColliderComponent* playerCollider =
        registry.GetCollider(player);
    HealthComponent* playerHealth =
        registry.GetHealth(player);

    TransformComponent* obstacleTransform =
        registry.GetTransform(obstacle);
    ColliderComponent* obstacleCollider =
        registry.GetCollider(obstacle);

    if (!playerTransform || !playerCollider || !playerHealth)
    {
        return;
    }

    const float playerCenterX =
        playerTransform->x + playerCollider->width * 0.5f;
    const float playerCenterY =
        playerTransform->y + playerCollider->height * 0.5f;

    for (Entity enemy : enemies)
    {
        TransformComponent* transform =
            registry.GetTransform(enemy);
        VelocityComponent* velocity =
            registry.GetVelocity(enemy);
        ColliderComponent* collider =
            registry.GetCollider(enemy);
        EnemyComponent* enemyData =
            registry.GetEnemy(enemy);

        if (!transform || !velocity || !collider || !enemyData)
        {
            continue;
        }

        enemyData->attackTimer = std::max(
            0.0f,
            enemyData->attackTimer - deltaTime
        );

        const float previousX = transform->x;
        const float previousY = transform->y;

        const float enemyCenterX =
            transform->x + collider->width * 0.5f;
        const float enemyCenterY =
            transform->y + collider->height * 0.5f;

        float directionX = playerCenterX - enemyCenterX;
        float directionY = playerCenterY - enemyCenterY;

        const float length = std::sqrt(
            directionX * directionX + directionY * directionY
        );

        if (length > 0.001f)
        {
            directionX /= length;
            directionY /= length;

            velocity->x = directionX * enemyData->moveSpeed;
            velocity->y = directionY * enemyData->moveSpeed;
        }
        else
        {
            velocity->x = 0.0f;
            velocity->y = 0.0f;
        }

        transform->x += velocity->x * deltaTime;
        transform->y += velocity->y * deltaTime;

        transform->x = std::clamp(
            transform->x,
            0.0f,
            WorldWidth - collider->width
        );

        transform->y = std::clamp(
            transform->y,
            0.0f,
            WorldHeight - collider->height
        );

        if (
            obstacleTransform &&
            obstacleCollider &&
            Physics::CheckCollision(
                *transform,
                *collider,
                *obstacleTransform,
                *obstacleCollider
            )
        )
        {
            transform->x = previousX;
            transform->y = previousY;
        }

        if (
            Physics::CheckCollision(
                *transform,
                *collider,
                *playerTransform,
                *playerCollider
            ) &&
            enemyData->attackTimer <= 0.0f
        )
        {
            playerHealth->current -= enemyData->contactDamage;
            enemyData->attackTimer = enemyData->attackCooldown;

            std::cout
                << "Player health: "
                << playerHealth->current
                << "/"
                << playerHealth->maximum
                << std::endl;

            if (playerHealth->current <= 0)
            {
                std::cout << "Game Over" << std::endl;
                running = false;
                return;
            }
        }
    }
}

void Application::UpdatePlayerAnimation(float deltaTime)
{
    SpriteComponent* sprite =
        registry.GetSprite(player);
    AnimationComponent* animation =
        registry.GetAnimation(player);

    if (!sprite || !animation)
    {
        return;
    }

    animation->elapsedTime += deltaTime;

    while (animation->elapsedTime >= animation->frameDuration)
    {
        animation->elapsedTime -= animation->frameDuration;
        sprite->currentFrame++;

        if (sprite->currentFrame >= animation->frameCount)
        {
            sprite->currentFrame =
                animation->looping
                    ? 0
                    : animation->frameCount - 1;
        }
    }
}

void Application::Render()
{
    renderer.BeginFrame();

    // World grid
    renderer.SetDrawColor(38, 38, 52);

    constexpr float gridSize = 100.0f;

    for (float worldX = 0.0f; worldX <= WorldWidth; worldX += gridSize)
    {
        const float screenX = camera.WorldToScreenX(worldX);

        if (screenX >= 0.0f && screenX <= ScreenWidth)
        {
            renderer.DrawLine(
                screenX,
                0.0f,
                screenX,
                ScreenHeight
            );
        }
    }

    for (float worldY = 0.0f; worldY <= WorldHeight; worldY += gridSize)
    {
        const float screenY = camera.WorldToScreenY(worldY);

        if (screenY >= 0.0f && screenY <= ScreenHeight)
        {
            renderer.DrawLine(
                0.0f,
                screenY,
                ScreenWidth,
                screenY
            );
        }
    }

    // Obstacle
    TransformComponent* obstacleTransform =
        registry.GetTransform(obstacle);
    ColliderComponent* obstacleCollider =
        registry.GetCollider(obstacle);

    if (obstacleTransform && obstacleCollider)
    {
        renderer.SetDrawColor(80, 80, 100);
        renderer.FillRectangle(
            camera.WorldToScreenX(obstacleTransform->x),
            camera.WorldToScreenY(obstacleTransform->y),
            obstacleCollider->width,
            obstacleCollider->height
        );
    }

    // Bullets
    renderer.SetDrawColor(255, 205, 70);

    for (Entity bullet : bullets)
    {
        TransformComponent* transform =
            registry.GetTransform(bullet);
        ColliderComponent* collider =
            registry.GetCollider(bullet);

        if (transform && collider)
        {
            renderer.FillRectangle(
                camera.WorldToScreenX(transform->x),
                camera.WorldToScreenY(transform->y),
                collider->width,
                collider->height
            );
        }
    }

    // Enemies
    for (Entity enemy : enemies)
    {
        TransformComponent* transform =
            registry.GetTransform(enemy);
        ColliderComponent* collider =
            registry.GetCollider(enemy);
        HealthComponent* health =
            registry.GetHealth(enemy);

        if (!transform || !collider || !health)
        {
            continue;
        }

        renderer.SetDrawColor(210, 65, 65);
        renderer.FillRectangle(
            camera.WorldToScreenX(transform->x),
            camera.WorldToScreenY(transform->y),
            collider->width,
            collider->height
        );

        const float healthRatio = std::clamp(
            static_cast<float>(health->current) /
                static_cast<float>(health->maximum),
            0.0f,
            1.0f
        );

        renderer.SetDrawColor(30, 30, 30);
        renderer.FillRectangle(
            camera.WorldToScreenX(transform->x),
            camera.WorldToScreenY(transform->y) - 8.0f,
            collider->width,
            4.0f
        );

        renderer.SetDrawColor(90, 220, 110);
        renderer.FillRectangle(
            camera.WorldToScreenX(transform->x),
            camera.WorldToScreenY(transform->y) - 8.0f,
            collider->width * healthRatio,
            4.0f
        );
    }

    // Player
    TransformComponent* playerTransform =
        registry.GetTransform(player);
    ColliderComponent* playerCollider =
        registry.GetCollider(player);
    SpriteComponent* playerSprite =
        registry.GetSprite(player);

    if (playerTransform && playerCollider)
    {
        const float screenX =
            camera.WorldToScreenX(playerTransform->x);
        const float screenY =
            camera.WorldToScreenY(playerTransform->y);

        SDL_Texture* playerTexture = nullptr;

        if (playerSprite)
        {
            playerTexture = textureManager.GetTexture(
                playerSprite->textureId
            );
        }

        if (playerTexture && playerSprite)
        {
            SDL_FRect source{
                static_cast<float>(
                    playerSprite->currentFrame *
                    playerSprite->frameWidth
                ),
                0.0f,
                static_cast<float>(playerSprite->frameWidth),
                static_cast<float>(playerSprite->frameHeight)
            };

            SDL_FRect destination{
                screenX,
                screenY,
                playerSprite->width,
                playerSprite->height
            };

            renderer.DrawTexture(
                playerTexture,
                &source,
                &destination
            );
        }
        else
        {
            renderer.SetDrawColor(235, 235, 245);
            renderer.FillRectangle(
                screenX,
                screenY,
                playerCollider->width,
                playerCollider->height
            );
        }

        // Aim line
        float mouseX = 0.0f;
        float mouseY = 0.0f;
        input.GetMousePosition(mouseX, mouseY);

        renderer.SetDrawColor(120, 180, 255);
        renderer.DrawLine(
            screenX + playerCollider->width * 0.5f,
            screenY + playerCollider->height * 0.5f,
            mouseX,
            mouseY
        );
    }

    // Player health bar (screen-space UI preview)
    HealthComponent* playerHealth =
        registry.GetHealth(player);

    if (playerHealth)
    {
        const float healthRatio = std::clamp(
            static_cast<float>(playerHealth->current) /
                static_cast<float>(playerHealth->maximum),
            0.0f,
            1.0f
        );

        renderer.SetDrawColor(45, 45, 55);
        renderer.FillRectangle(20.0f, 20.0f, 220.0f, 22.0f);

        renderer.SetDrawColor(90, 220, 110);
        renderer.FillRectangle(
            23.0f,
            23.0f,
            214.0f * healthRatio,
            16.0f
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
