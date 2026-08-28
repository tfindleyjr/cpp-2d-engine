#pragma once

struct AnimationComponent
{
    int frameCount = 1;

    float frameDuration = 0.1f;
    float elapsedTime = 0.0f;

    bool looping = true;
};
