#pragma once

#include <SDL3/SDL.h>

class AudioSystem
{
public:
    AudioSystem();
    ~AudioSystem();

    bool Initialize();
    void Shutdown();

    void PlayShoot();
    void PlayHit();
    void PlayPlayerDamage();
    void PlayWaveStart();

private:
    void PlayTone(float frequency, float duration, float volume);

    SDL_AudioStream* stream;
    bool enabled;
};
