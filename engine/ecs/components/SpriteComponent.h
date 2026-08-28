#pragma once

#include <string>

struct SpriteComponent
{
    std::string textureId;

    float width = 0.0f;
    float height = 0.0f;

    int frameWidth = 0;
    int frameHeight = 0;

    int currentFrame = 0;
};
