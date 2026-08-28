#pragma once

#include <cstddef>

class EngineDiagnostics
{
public:
    void Update(
        float deltaTime,
        std::size_t liveEntities,
        std::size_t bullets,
        std::size_t enemies
    );

    void ToggleEnabled();
    void ToggleColliders();
    void ToggleGrid();

    bool IsEnabled() const;
    bool ShowColliders() const;
    bool ShowGrid() const;

    float GetFramesPerSecond() const;
    float GetAverageFrameMilliseconds() const;
    std::size_t GetLiveEntities() const;
    std::size_t GetBulletCount() const;
    std::size_t GetEnemyCount() const;

private:
    bool enabled = false;
    bool showColliders = false;
    bool showGrid = true;

    float sampleTimer = 0.0f;
    float accumulatedTime = 0.0f;
    int accumulatedFrames = 0;

    float framesPerSecond = 0.0f;
    float averageFrameMilliseconds = 0.0f;

    std::size_t liveEntities = 0;
    std::size_t bulletCount = 0;
    std::size_t enemyCount = 0;
};
