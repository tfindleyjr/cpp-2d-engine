#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <unordered_map>

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    bool LoadTexture(
        SDL_Renderer* renderer,
        const std::string& id,
        const std::string& filePath
    );

    SDL_Texture* GetTexture(
        const std::string& id
    );

    void Shutdown();

private:
    std::unordered_map<
        std::string,
        SDL_Texture*
    > textures;
};