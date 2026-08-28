#include "EngineDiagnostics.h"

void EngineDiagnostics::Update(
    float deltaTime,
    std::size_t newLiveEntities,
    std::size_t bullets,
    std::size_t enemies
)
{
    liveEntities = newLiveEntities;
    bulletCount = bullets;
    enemyCount = enemies;

    sampleTimer += deltaTime;
    accumulatedTime += deltaTime;
    ++accumulatedFrames;

    if (sampleTimer < 0.5f)
    {
        return;
    }

    if (accumulatedTime > 0.0f && accumulatedFrames > 0)
    {
        framesPerSecond =
            static_cast<float>(accumulatedFrames) / accumulatedTime;

        averageFrameMilliseconds =
            (accumulatedTime * 1000.0f) /
            static_cast<float>(accumulatedFrames);
    }

    sampleTimer = 0.0f;
    accumulatedTime = 0.0f;
    accumulatedFrames = 0;
}

void EngineDiagnostics::ToggleEnabled()
{
    enabled = !enabled;
}

void EngineDiagnostics::ToggleColliders()
{
    showColliders = !showColliders;
}

void EngineDiagnostics::ToggleGrid()
{
    showGrid = !showGrid;
}

bool EngineDiagnostics::IsEnabled() const
{
    return enabled;
}

bool EngineDiagnostics::ShowColliders() const
{
    return showColliders;
}

bool EngineDiagnostics::ShowGrid() const
{
    return showGrid;
}

float EngineDiagnostics::GetFramesPerSecond() const
{
    return framesPerSecond;
}

float EngineDiagnostics::GetAverageFrameMilliseconds() const
{
    return averageFrameMilliseconds;
}

std::size_t EngineDiagnostics::GetLiveEntities() const
{
    return liveEntities;
}

std::size_t EngineDiagnostics::GetBulletCount() const
{
    return bulletCount;
}

std::size_t EngineDiagnostics::GetEnemyCount() const
{
    return enemyCount;
}
