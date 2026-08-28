#pragma once

class GameFeel
{
public:
    GameFeel();

    void Reset();
    void Update(float deltaTime);

    void TriggerShoot();
    void TriggerHit();
    void TriggerPlayerDamage();

    float GetShakeX() const;
    float GetShakeY() const;
    float GetHitFlashAlpha() const;
    float GetDamageFlashAlpha() const;

private:
    float elapsed;
    float shakeTime;
    float shakeStrength;
    float hitFlash;
    float damageFlash;
};
