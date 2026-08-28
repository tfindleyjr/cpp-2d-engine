#pragma once

#include <SDL3/SDL.h>

#include <cstddef>
#include <vector>

#include "engine/arena/ArenaManager.h"
#include "engine/audio/AudioSystem.h"
#include "engine/core/GameState.h"
#include "engine/debug/EngineDiagnostics.h"
#include "engine/effects/GameFeel.h"
#include "engine/ecs/Registry.h"
#include "engine/input/Input.h"
#include "engine/renderer/Renderer.h"
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

    void ResetGame();
    void CreateWorld();
    void UpdateArena(float deltaTime);
    void UpdatePlayer(float deltaTime);
    void UpdateBullets(float deltaTime);
    void UpdateEnemies(float deltaTime);
    void UpdatePlayerAnimation(float deltaTime);

    void SpawnBullet();
    void SpawnEnemy(float x, float y);
    void SpawnEnemyAtArenaEdge();

    void RenderGrid(float shakeX, float shakeY);
    void RenderWorld(float shakeX, float shakeY);
    void RenderUI();
    void RenderDebug(float shakeX, float shakeY);
    void UpdateWindowTitle();

    bool IsWorldRectVisible(
        float x,
        float y,
        float width,
        float height,
        float margin = 32.0f
    ) const;

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
    ArenaManager arena;
    AudioSystem audio;
    GameFeel gameFeel;
    EngineDiagnostics diagnostics;

    Entity player;
    Entity obstacle;

    std::vector<Entity> bullets;
    std::vector<Entity> enemies;

    GameState gameState;
    bool running;
    Uint64 lastFrameTime;

    float shootCooldown;
    std::size_t spawnIndex;
    int lastAnnouncedWave;
    int score;
};
