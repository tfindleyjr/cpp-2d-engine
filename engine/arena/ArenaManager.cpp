#include "ArenaManager.h"

#include <algorithm>

ArenaManager::ArenaManager()
{
    Reset();
}

void ArenaManager::Reset()
{
    wave = 0;
    enemiesRemainingToSpawn = 0;
    spawnTimer = 0.0f;
    spawnInterval = 1.0f;
    betweenWaveTimer = 1.5f;
    waveActive = false;
}

void ArenaManager::BeginNextWave()
{
    ++wave;
    enemiesRemainingToSpawn = 3 + wave * 2;
    spawnInterval = std::max(0.35f, 1.0f - static_cast<float>(wave) * 0.07f);
    spawnTimer = 0.0f;
    waveActive = true;
}

void ArenaManager::Update(float deltaTime, std::size_t aliveEnemies)
{
    if (!waveActive)
    {
        betweenWaveTimer -= deltaTime;
        if (betweenWaveTimer <= 0.0f)
        {
            BeginNextWave();
        }
        return;
    }

    if (enemiesRemainingToSpawn > 0)
    {
        spawnTimer -= deltaTime;
    }

    if (enemiesRemainingToSpawn == 0 && aliveEnemies == 0)
    {
        waveActive = false;
        betweenWaveTimer = 2.5f;
    }
}

bool ArenaManager::ShouldSpawnEnemy()
{
    if (!waveActive || enemiesRemainingToSpawn <= 0 || spawnTimer > 0.0f)
    {
        return false;
    }

    --enemiesRemainingToSpawn;
    spawnTimer = spawnInterval;
    return true;
}

int ArenaManager::GetWave() const
{
    return wave;
}

bool ArenaManager::IsBetweenWaves() const
{
    return !waveActive;
}

float ArenaManager::GetEnemySpeed() const
{
    return 105.0f + static_cast<float>(wave) * 8.0f;
}

int ArenaManager::GetEnemyHealth() const
{
    return 2 + (wave + 1) / 2;
}
