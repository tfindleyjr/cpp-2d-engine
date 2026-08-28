#pragma once

#include <SDL3/SDL.h>

#include <vector>

#include "engine/renderer/Renderer.h"
#include "engine/input/Input.h"
#include "engine/ecs/Registry.h"
#include "engine/resources/TextureManager.h"
#include "engine/world/Camera.h"

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

    void UpdatePlayer(float deltaTime);
    void UpdateBullets(float deltaTime);
    void UpdateEnemies(float deltaTime);
    void UpdatePlayerAnimation(float deltaTime);

    void SpawnBullet();
    void SpawnEnemy(float x, float y);

private:
    static constexpr float ScreenWidth = 1280.0f;
    static constexpr float ScreenHeight = 720.0f;
    static constexpr float WorldWidth = 2400.0f;
    static constexpr float WorldHeight = 1600.0f;

    SDL_Window* window;

    Renderer renderer;
    Input input;
    TextureManager textureManager;
    Registry registry;
    Camera camera;

    Entity player;
    Entity obstacle;

    std::vector<Entity> bullets;
    std::vector<Entity> enemies;

    bool running;
    Uint64 lastFrameTime;

    float shootCooldown;
};
