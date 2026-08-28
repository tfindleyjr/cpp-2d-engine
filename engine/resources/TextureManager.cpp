#include "TextureManager.h"

#include <iostream>

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    Shutdown();
}

bool TextureManager::LoadTexture(
    SDL_Renderer* renderer,
    const std::string& id,
    const std::string& filePath
)
{
    SDL_Surface* surface =
        SDL_LoadBMP(filePath.c_str());

    if (!surface)
    {
        std::cerr
            << "Failed to load image: "
            << filePath
            << "\nSDL error: "
            << SDL_GetError()
            << std::endl;

        return false;
    }

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_DestroySurface(surface);

    if (!texture)
    {
        std::cerr
            << "Failed to create texture: "
            << filePath
            << "\nSDL error: "
            << SDL_GetError()
            << std::endl;

        return false;
    }

    textures[id] = texture;

    std::cout
        << "Loaded texture: "
        << id
        << std::endl;

    return true;
}

SDL_Texture* TextureManager::GetTexture(
    const std::string& id
)
{
    auto it =
        textures.find(id);

    if (it == textures.end())
    {
        return nullptr;
    }

    return it->second;
}

void TextureManager::Shutdown()
{
    for (auto& pair : textures)
    {
        if (pair.second)
        {
            SDL_DestroyTexture(
                pair.second
            );
        }
    }

    textures.clear();
}