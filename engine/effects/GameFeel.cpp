#include "GameFeel.h"

#include <algorithm>
#include <cmath>

GameFeel::GameFeel()
{
    Reset();
}

void GameFeel::Reset()
{
    elapsed = 0.0f;
    shakeTime = 0.0f;
    shakeStrength = 0.0f;
    hitFlash = 0.0f;
    damageFlash = 0.0f;
}

void GameFeel::Update(float deltaTime)
{
    elapsed += deltaTime;
    shakeTime = std::max(0.0f, shakeTime - deltaTime);
    hitFlash = std::max(0.0f, hitFlash - deltaTime * 4.0f);
    damageFlash = std::max(0.0f, damageFlash - deltaTime * 2.8f);

    if (shakeTime <= 0.0f)
    {
        shakeStrength = 0.0f;
    }
}

void GameFeel::TriggerShoot()
{
    shakeTime = std::max(shakeTime, 0.04f);
    shakeStrength = std::max(shakeStrength, 1.5f);
}

void GameFeel::TriggerHit()
{
    shakeTime = std::max(shakeTime, 0.08f);
    shakeStrength = std::max(shakeStrength, 3.0f);
    hitFlash = 0.12f;
}

void GameFeel::TriggerPlayerDamage()
{
    shakeTime = 0.20f;
    shakeStrength = 8.0f;
    damageFlash = 0.24f;
}

float GameFeel::GetShakeX() const
{
    return shakeTime > 0.0f ? std::sin(elapsed * 95.0f) * shakeStrength : 0.0f;
}

float GameFeel::GetShakeY() const
{
    return shakeTime > 0.0f ? std::cos(elapsed * 117.0f) * shakeStrength : 0.0f;
}

float GameFeel::GetHitFlashAlpha() const
{
    return std::clamp(hitFlash / 0.12f, 0.0f, 1.0f);
}

float GameFeel::GetDamageFlashAlpha() const
{
    return std::clamp(damageFlash / 0.24f, 0.0f, 1.0f);
}
