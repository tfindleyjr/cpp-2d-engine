#include "AudioSystem.h"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

AudioSystem::AudioSystem()
    : stream(nullptr),
      enabled(false)
{
}

AudioSystem::~AudioSystem()
{
    Shutdown();
}

bool AudioSystem::Initialize()
{
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        std::cout << "Audio unavailable: " << SDL_GetError() << std::endl;
        return false;
    }

    const SDL_AudioSpec spec{
        SDL_AUDIO_S16,
        1,
        44100
    };

    stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec,
        nullptr,
        nullptr
    );

    if (!stream)
    {
        std::cout << "Audio stream unavailable: " << SDL_GetError() << std::endl;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }

    if (!SDL_ResumeAudioStreamDevice(stream))
    {
        std::cout << "Could not resume audio stream: " << SDL_GetError() << std::endl;
        SDL_DestroyAudioStream(stream);
        stream = nullptr;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }

    enabled = true;
    return true;
}

void AudioSystem::Shutdown()
{
    if (stream)
    {
        SDL_DestroyAudioStream(stream);
        stream = nullptr;
    }

    if (enabled)
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    enabled = false;
}

void AudioSystem::PlayTone(float frequency, float duration, float volume)
{
    if (!enabled || !stream)
    {
        return;
    }

    constexpr int sampleRate = 44100;
    constexpr float pi = 3.14159265358979323846f;

    const int sampleCount = static_cast<int>(duration * sampleRate);
    std::vector<std::int16_t> samples(sampleCount);

    for (int i = 0; i < sampleCount; ++i)
    {
        const float time = static_cast<float>(i) / static_cast<float>(sampleRate);
        const float envelope = 1.0f - static_cast<float>(i) / static_cast<float>(sampleCount);
        const float wave = std::sin(2.0f * pi * frequency * time);
        samples[i] = static_cast<std::int16_t>(wave * envelope * volume * 32767.0f);
    }

    SDL_PutAudioStreamData(
        stream,
        samples.data(),
        static_cast<int>(samples.size() * sizeof(std::int16_t))
    );
}

void AudioSystem::PlayShoot()
{
    PlayTone(520.0f, 0.055f, 0.22f);
}

void AudioSystem::PlayHit()
{
    PlayTone(760.0f, 0.045f, 0.18f);
}

void AudioSystem::PlayPlayerDamage()
{
    PlayTone(150.0f, 0.16f, 0.30f);
}

void AudioSystem::PlayWaveStart()
{
    PlayTone(320.0f, 0.10f, 0.18f);
    PlayTone(480.0f, 0.10f, 0.18f);
}
