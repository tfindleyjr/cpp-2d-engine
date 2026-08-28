#pragma once

#include <cstddef>

class ArenaManager
{
public:
    ArenaManager();

    void Reset();
    void Update(float deltaTime, std::size_t aliveEnemies);
    bool ShouldSpawnEnemy();

    int GetWave() const;
    bool IsBetweenWaves() const;
    float GetEnemySpeed() const;
    int GetEnemyHealth() const;

private:
    void BeginNextWave();

    int wave;
    int enemiesRemainingToSpawn;
    float spawnTimer;
    float spawnInterval;
    float betweenWaveTimer;
    bool waveActive;
};
