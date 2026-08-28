#include "Application.h"

#include "engine/physics/Physics.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>

Application::Application()
    : window(nullptr),
      camera(ScreenWidth, ScreenHeight),
      player(0),
      obstacle(0),
      gameState(GameState::Playing),
      running(false),
      lastFrameTime(0),
      shootCooldown(0.0f),
      spawnIndex(0),
      lastAnnouncedWave(0),
      score(0)
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

    // Audio is optional. The game remains playable on systems without an
    // available playback device.
    audio.Initialize();

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
    arena.Reset();
    gameFeel.Reset();
    CreateWorld();

    running = true;
    lastFrameTime = SDL_GetPerformanceCounter();

    std::cout << "Engine initialized successfully." << std::endl;
    std::cout
        << "Controls: WASD move, mouse aim, left click shoot, P pause, "
        << "R restart after death, F1 diagnostics, F2 colliders, F3 grid, ESC quit."
        << std::endl;

    return true;
}

void Application::CreateWorld()
{
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

    obstacle = registry.CreateEntity();

    registry.AddTransform(
        obstacle,
        TransformComponent{950.0f, 620.0f}
    );

    registry.AddCollider(
        obstacle,
        ColliderComponent{500.0f, 70.0f}
    );

    TransformComponent* playerTransform = registry.GetTransform(player);
    ColliderComponent* playerCollider = registry.GetCollider(player);

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

void Application::ResetGame()
{
    for (Entity bullet : bullets)
    {
        registry.DestroyEntity(bullet);
    }

    for (Entity enemy : enemies)
    {
        registry.DestroyEntity(enemy);
    }

    bullets.clear();
    enemies.clear();

    registry.DestroyEntity(player);
    registry.DestroyEntity(obstacle);

    arena.Reset();
    gameFeel.Reset();

    shootCooldown = 0.0f;
    spawnIndex = 0;
    lastAnnouncedWave = 0;
    score = 0;
    gameState = GameState::Playing;

    CreateWorld();
}

void Application::Run()
{
    while (running)
    {
        const Uint64 currentFrameTime = SDL_GetPerformanceCounter();
        const double frequency =
            static_cast<double>(SDL_GetPerformanceFrequency());

        float deltaTime = static_cast<float>(
            (currentFrameTime - lastFrameTime) / frequency
        );

        lastFrameTime = currentFrameTime;
        deltaTime = std::min(deltaTime, 0.05f);

        diagnostics.Update(
            deltaTime,
            registry.AliveCount(),
            bullets.size(),
            enemies.size()
        );

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
        return;
    }

    if (input.WasKeyPressed(SDL_SCANCODE_F1))
    {
        diagnostics.ToggleEnabled();
    }

    if (input.WasKeyPressed(SDL_SCANCODE_F2))
    {
        diagnostics.ToggleColliders();
    }

    if (input.WasKeyPressed(SDL_SCANCODE_F3))
    {
        diagnostics.ToggleGrid();
    }

    if (
        input.WasKeyPressed(SDL_SCANCODE_P) &&
        gameState != GameState::GameOver
    )
    {
        gameState =
            gameState == GameState::Paused
                ? GameState::Playing
                : GameState::Paused;
    }

    if (
        input.WasKeyPressed(SDL_SCANCODE_R) &&
        gameState == GameState::GameOver
    )
    {
        ResetGame();
    }
}

void Application::Update(float deltaTime)
{
    gameFeel.Update(deltaTime);

    if (gameState != GameState::Playing)
    {
        UpdateWindowTitle();
        return;
    }

    if (shootCooldown > 0.0f)
    {
        shootCooldown -= deltaTime;
    }

    UpdateArena(deltaTime);
    UpdatePlayer(deltaTime);
    UpdateEnemies(deltaTime);
    UpdateBullets(deltaTime);
    UpdatePlayerAnimation(deltaTime);

    HealthComponent* playerHealth = registry.GetHealth(player);

    if (playerHealth && playerHealth->current <= 0)
    {
        gameState = GameState::GameOver;
    }

    TransformComponent* playerTransform = registry.GetTransform(player);
    ColliderComponent* playerCollider = registry.GetCollider(player);

    if (playerTransform && playerCollider)
    {
        camera.Follow(
            playerTransform->x,
            playerTransform->y,
            playerCollider->width,
            playerCollider->height
        );
    }

    UpdateWindowTitle();
}

void Application::UpdateArena(float deltaTime)
{
    arena.Update(deltaTime, enemies.size());

    if (
        arena.GetWave() > 0 &&
        arena.GetWave() != lastAnnouncedWave
    )
    {
        lastAnnouncedWave = arena.GetWave();
        audio.PlayWaveStart();
        std::cout << "Wave " << lastAnnouncedWave << " started." << std::endl;
    }

    while (arena.ShouldSpawnEnemy())
    {
        SpawnEnemyAtArenaEdge();
    }
}

void Application::SpawnEnemyAtArenaEdge()
{
    static constexpr float positions[][2] = {
        {90.0f, 90.0f},
        {1150.0f, 90.0f},
        {2260.0f, 90.0f},
        {90.0f, 760.0f},
        {2260.0f, 760.0f},
        {90.0f, 1460.0f},
        {1150.0f, 1460.0f},
        {2260.0f, 1460.0f}
    };

    constexpr std::size_t positionCount =
        sizeof(positions) / sizeof(positions[0]);

    const std::size_t index = spawnIndex % positionCount;
    ++spawnIndex;

    SpawnEnemy(
        positions[index][0],
        positions[index][1]
    );
}

void Application::UpdatePlayer(float deltaTime)
{
    TransformComponent* transform = registry.GetTransform(player);
    VelocityComponent* velocity = registry.GetVelocity(player);
    ColliderComponent* collider = registry.GetCollider(player);
    HealthComponent* health = registry.GetHealth(player);

    if (!transform || !velocity || !collider || !health)
    {
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

    TransformComponent* obstacleTransform = registry.GetTransform(obstacle);
    ColliderComponent* obstacleCollider = registry.GetCollider(obstacle);

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
        audio.PlayShoot();
        gameFeel.TriggerShoot();
    }
}

void Application::SpawnBullet()
{
    TransformComponent* playerTransform = registry.GetTransform(player);
    ColliderComponent* playerCollider = registry.GetCollider(player);

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

    TransformComponent* obstacleTransform = registry.GetTransform(obstacle);
    ColliderComponent* obstacleCollider = registry.GetCollider(obstacle);

    for (Entity bullet : bullets)
    {
        TransformComponent* transform = registry.GetTransform(bullet);
        VelocityComponent* velocity = registry.GetVelocity(bullet);
        ColliderComponent* collider = registry.GetCollider(bullet);
        ProjectileComponent* projectile = registry.GetProjectile(bullet);

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

            TransformComponent* enemyTransform = registry.GetTransform(enemy);
            ColliderComponent* enemyCollider = registry.GetCollider(enemy);
            HealthComponent* enemyHealth = registry.GetHealth(enemy);

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
                gameFeel.TriggerHit();
                audio.PlayHit();

                if (enemyHealth->current <= 0)
                {
                    deadEnemies.insert(enemy);
                    score += 100;
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
    const int health = arena.GetEnemyHealth();

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
        HealthComponent{health, health}
    );

    registry.AddEnemy(
        enemy,
        EnemyComponent{
            arena.GetEnemySpeed(),
            1,
            0.75f,
            0.0f
        }
    );

    enemies.push_back(enemy);
}

void Application::UpdateEnemies(float deltaTime)
{
    TransformComponent* playerTransform = registry.GetTransform(player);
    ColliderComponent* playerCollider = registry.GetCollider(player);
    HealthComponent* playerHealth = registry.GetHealth(player);

    TransformComponent* obstacleTransform = registry.GetTransform(obstacle);
    ColliderComponent* obstacleCollider = registry.GetCollider(obstacle);

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
        TransformComponent* transform = registry.GetTransform(enemy);
        VelocityComponent* velocity = registry.GetVelocity(enemy);
        ColliderComponent* collider = registry.GetCollider(enemy);
        EnemyComponent* enemyData = registry.GetEnemy(enemy);

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

        if (Physics::CheckCollision(
                *transform,
                *collider,
                *playerTransform,
                *playerCollider
            ))
        {
            transform->x = previousX;
            transform->y = previousY;

            if (enemyData->attackTimer <= 0.0f)
            {
                playerHealth->current -= enemyData->contactDamage;
                enemyData->attackTimer = enemyData->attackCooldown;
                gameFeel.TriggerPlayerDamage();
                audio.PlayPlayerDamage();

                if (playerHealth->current <= 0)
                {
                    gameState = GameState::GameOver;
                    return;
                }
            }
        }
    }
}

void Application::UpdatePlayerAnimation(float deltaTime)
{
    SpriteComponent* sprite = registry.GetSprite(player);
    AnimationComponent* animation = registry.GetAnimation(player);
    VelocityComponent* velocity = registry.GetVelocity(player);

    if (!sprite || !animation || !velocity)
    {
        return;
    }

    const bool moving =
        std::abs(velocity->x) > 0.01f ||
        std::abs(velocity->y) > 0.01f;

    if (!moving)
    {
        sprite->currentFrame = 0;
        animation->elapsedTime = 0.0f;
        return;
    }

    animation->elapsedTime += deltaTime;

    while (animation->elapsedTime >= animation->frameDuration)
    {
        animation->elapsedTime -= animation->frameDuration;
        ++sprite->currentFrame;

        if (sprite->currentFrame >= animation->frameCount)
        {
            sprite->currentFrame = animation->looping
                ? 0
                : animation->frameCount - 1;
        }
    }
}

bool Application::IsWorldRectVisible(
    float x,
    float y,
    float width,
    float height,
    float margin
) const
{
    const float left = camera.GetX() - margin;
    const float top = camera.GetY() - margin;
    const float right = camera.GetX() + ScreenWidth + margin;
    const float bottom = camera.GetY() + ScreenHeight + margin;

    return
        x + width >= left &&
        x <= right &&
        y + height >= top &&
        y <= bottom;
}

void Application::RenderGrid(float shakeX, float shakeY)
{
    constexpr float gridSize = 100.0f;

    renderer.SetDrawColor(38, 38, 52);

    for (float x = 0.0f; x <= WorldWidth; x += gridSize)
    {
        const float screenX = camera.WorldToScreenX(x) + shakeX;

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

    for (float y = 0.0f; y <= WorldHeight; y += gridSize)
    {
        const float screenY = camera.WorldToScreenY(y) + shakeY;

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
}

void Application::RenderWorld(float shakeX, float shakeY)
{
    TransformComponent* obstacleTransform = registry.GetTransform(obstacle);
    ColliderComponent* obstacleCollider = registry.GetCollider(obstacle);

    if (
        obstacleTransform &&
        obstacleCollider &&
        IsWorldRectVisible(
            obstacleTransform->x,
            obstacleTransform->y,
            obstacleCollider->width,
            obstacleCollider->height
        )
    )
    {
        renderer.SetDrawColor(80, 80, 100);
        renderer.FillRectangle(
            camera.WorldToScreenX(obstacleTransform->x) + shakeX,
            camera.WorldToScreenY(obstacleTransform->y) + shakeY,
            obstacleCollider->width,
            obstacleCollider->height
        );
    }

    renderer.SetDrawColor(245, 210, 65);

    for (Entity bullet : bullets)
    {
        TransformComponent* transform = registry.GetTransform(bullet);
        ColliderComponent* collider = registry.GetCollider(bullet);

        if (
            transform &&
            collider &&
            IsWorldRectVisible(
                transform->x,
                transform->y,
                collider->width,
                collider->height
            )
        )
        {
            renderer.FillRectangle(
                camera.WorldToScreenX(transform->x) + shakeX,
                camera.WorldToScreenY(transform->y) + shakeY,
                collider->width,
                collider->height
            );
        }
    }

    for (Entity enemy : enemies)
    {
        TransformComponent* transform = registry.GetTransform(enemy);
        ColliderComponent* collider = registry.GetCollider(enemy);
        HealthComponent* health = registry.GetHealth(enemy);

        if (!transform || !collider)
        {
            continue;
        }

        if (!IsWorldRectVisible(
                transform->x,
                transform->y,
                collider->width,
                collider->height,
                48.0f
            ))
        {
            continue;
        }

        const float screenX = camera.WorldToScreenX(transform->x) + shakeX;
        const float screenY = camera.WorldToScreenY(transform->y) + shakeY;

        renderer.SetDrawColor(205, 62, 72);
        renderer.FillRectangle(
            screenX,
            screenY,
            collider->width,
            collider->height
        );

        if (health && health->maximum > 0)
        {
            const float ratio = std::clamp(
                static_cast<float>(health->current) /
                    static_cast<float>(health->maximum),
                0.0f,
                1.0f
            );

            renderer.SetDrawColor(45, 45, 50);
            renderer.FillRectangle(
                screenX,
                screenY - 8.0f,
                collider->width,
                4.0f
            );

            renderer.SetDrawColor(90, 220, 105);
            renderer.FillRectangle(
                screenX,
                screenY - 8.0f,
                collider->width * ratio,
                4.0f
            );
        }
    }

    TransformComponent* playerTransform = registry.GetTransform(player);
    ColliderComponent* playerCollider = registry.GetCollider(player);
    SpriteComponent* playerSprite = registry.GetSprite(player);

    if (!playerTransform || !playerCollider)
    {
        return;
    }

    const float playerScreenX =
        camera.WorldToScreenX(playerTransform->x) + shakeX;
    const float playerScreenY =
        camera.WorldToScreenY(playerTransform->y) + shakeY;

    SDL_Texture* playerTexture = nullptr;

    if (playerSprite)
    {
        playerTexture = textureManager.GetTexture(playerSprite->textureId);
    }

    if (playerSprite && playerTexture)
    {
        SDL_FRect source{
            static_cast<float>(
                playerSprite->currentFrame * playerSprite->frameWidth
            ),
            0.0f,
            static_cast<float>(playerSprite->frameWidth),
            static_cast<float>(playerSprite->frameHeight)
        };

        SDL_FRect destination{
            playerScreenX,
            playerScreenY,
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
            playerScreenX,
            playerScreenY,
            playerCollider->width,
            playerCollider->height
        );
    }
}

void Application::RenderDebug(float shakeX, float shakeY)
{
    if (!diagnostics.ShowColliders())
    {
        return;
    }

    renderer.SetDrawColor(70, 225, 235);

    const auto drawCollider = [this, shakeX, shakeY](
        Entity entity,
        float margin
    )
    {
        TransformComponent* transform = registry.GetTransform(entity);
        ColliderComponent* collider = registry.GetCollider(entity);

        if (!transform || !collider)
        {
            return;
        }

        if (!IsWorldRectVisible(
                transform->x,
                transform->y,
                collider->width,
                collider->height,
                margin
            ))
        {
            return;
        }

        renderer.DrawRectangle(
            camera.WorldToScreenX(transform->x) + shakeX,
            camera.WorldToScreenY(transform->y) + shakeY,
            collider->width,
            collider->height
        );
    };

    drawCollider(player, 64.0f);
    drawCollider(obstacle, 64.0f);

    for (Entity bullet : bullets)
    {
        drawCollider(bullet, 32.0f);
    }

    for (Entity enemy : enemies)
    {
        drawCollider(enemy, 64.0f);
    }

    renderer.SetDrawColor(110, 180, 255);
    renderer.DrawLine(
        ScreenWidth * 0.5f - 10.0f,
        ScreenHeight * 0.5f,
        ScreenWidth * 0.5f + 10.0f,
        ScreenHeight * 0.5f
    );
    renderer.DrawLine(
        ScreenWidth * 0.5f,
        ScreenHeight * 0.5f - 10.0f,
        ScreenWidth * 0.5f,
        ScreenHeight * 0.5f + 10.0f
    );
}

void Application::RenderUI()
{
    HealthComponent* health = registry.GetHealth(player);

    if (health && health->maximum > 0)
    {
        const float ratio = std::clamp(
            static_cast<float>(health->current) /
                static_cast<float>(health->maximum),
            0.0f,
            1.0f
        );

        renderer.SetDrawColor(35, 35, 42);
        renderer.FillRectangle(20.0f, 20.0f, 220.0f, 18.0f);

        renderer.SetDrawColor(75, 215, 105);
        renderer.FillRectangle(20.0f, 20.0f, 220.0f * ratio, 18.0f);

        renderer.SetDrawColor(225, 225, 235);
        renderer.DrawRectangle(20.0f, 20.0f, 220.0f, 18.0f);
    }

    const int waveBlocks = std::min(arena.GetWave(), 12);

    for (int i = 0; i < waveBlocks; ++i)
    {
        renderer.SetDrawColor(120, 155, 255);
        renderer.FillRectangle(
            520.0f + static_cast<float>(i) * 20.0f,
            20.0f,
            14.0f,
            14.0f
        );
    }

    float mouseX = 0.0f;
    float mouseY = 0.0f;
    input.GetMousePosition(mouseX, mouseY);

    renderer.SetDrawColor(245, 245, 245);
    renderer.DrawLine(mouseX - 8.0f, mouseY, mouseX + 8.0f, mouseY);
    renderer.DrawLine(mouseX, mouseY - 8.0f, mouseX, mouseY + 8.0f);

    SDL_Renderer* rawRenderer = renderer.GetSDLRenderer();
    SDL_SetRenderDrawBlendMode(rawRenderer, SDL_BLENDMODE_BLEND);

    const float hitAlpha = gameFeel.GetHitFlashAlpha();
    if (hitAlpha > 0.0f)
    {
        SDL_SetRenderDrawColor(
            rawRenderer,
            255,
            255,
            255,
            static_cast<Uint8>(hitAlpha * 55.0f)
        );
        const SDL_FRect overlay{0.0f, 0.0f, ScreenWidth, ScreenHeight};
        SDL_RenderFillRect(rawRenderer, &overlay);
    }

    const float damageAlpha = gameFeel.GetDamageFlashAlpha();
    if (damageAlpha > 0.0f)
    {
        SDL_SetRenderDrawColor(
            rawRenderer,
            220,
            30,
            35,
            static_cast<Uint8>(damageAlpha * 110.0f)
        );
        const SDL_FRect overlay{0.0f, 0.0f, ScreenWidth, ScreenHeight};
        SDL_RenderFillRect(rawRenderer, &overlay);
    }

    if (gameState == GameState::Paused)
    {
        SDL_SetRenderDrawColor(rawRenderer, 10, 10, 16, 175);
        const SDL_FRect overlay{0.0f, 0.0f, ScreenWidth, ScreenHeight};
        SDL_RenderFillRect(rawRenderer, &overlay);

        renderer.SetDrawColor(230, 230, 240);
        renderer.DrawRectangle(490.0f, 270.0f, 300.0f, 180.0f);
    }
    else if (gameState == GameState::GameOver)
    {
        SDL_SetRenderDrawColor(rawRenderer, 55, 5, 8, 185);
        const SDL_FRect overlay{0.0f, 0.0f, ScreenWidth, ScreenHeight};
        SDL_RenderFillRect(rawRenderer, &overlay);

        renderer.SetDrawColor(245, 95, 95);
        renderer.DrawRectangle(430.0f, 240.0f, 420.0f, 240.0f);
    }

    SDL_SetRenderDrawBlendMode(rawRenderer, SDL_BLENDMODE_NONE);
}

void Application::Render()
{
    renderer.BeginFrame();

    const float shakeX = gameFeel.GetShakeX();
    const float shakeY = gameFeel.GetShakeY();

    if (diagnostics.ShowGrid())
    {
        RenderGrid(shakeX, shakeY);
    }

    RenderWorld(shakeX, shakeY);
    RenderDebug(shakeX, shakeY);
    RenderUI();

    renderer.EndFrame();
}

void Application::UpdateWindowTitle()
{
    if (!window)
    {
        return;
    }

    HealthComponent* health = registry.GetHealth(player);
    const int currentHealth = health ? std::max(0, health->current) : 0;
    const int maxHealth = health ? health->maximum : 0;

    std::ostringstream title;
    title
        << "Arena Engine | Wave "
        << arena.GetWave()
        << " | HP "
        << currentHealth
        << "/"
        << maxHealth
        << " | Enemies "
        << enemies.size()
        << " | Score "
        << score;

    if (diagnostics.IsEnabled())
    {
        title
            << std::fixed
            << std::setprecision(1)
            << " | FPS "
            << diagnostics.GetFramesPerSecond()
            << " | "
            << diagnostics.GetAverageFrameMilliseconds()
            << " ms"
            << " | Entities "
            << diagnostics.GetLiveEntities()
            << " | Colliders "
            << (diagnostics.ShowColliders() ? "ON" : "OFF")
            << " | Grid "
            << (diagnostics.ShowGrid() ? "ON" : "OFF");
    }

    if (gameState == GameState::Paused)
    {
        title << " | PAUSED - press P to resume";
    }
    else if (gameState == GameState::GameOver)
    {
        title << " | GAME OVER - press R to restart";
    }

    SDL_SetWindowTitle(window, title.str().c_str());
}

void Application::Shutdown()
{
    audio.Shutdown();
    textureManager.Shutdown();
    renderer.Shutdown();

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}
