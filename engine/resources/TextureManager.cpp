#include "TextureManager.h"

#include <iostream>
#include <memory>

namespace
{
struct SDLSurfaceDeleter
{
    void operator()(SDL_Surface* surface) const
    {
        if (surface)
        {
            SDL_DestroySurface(surface);
        }
    }
};

using SurfacePtr = std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>;
}

bool TextureManager::LoadTexture(
    SDL_Renderer* renderer,
    const std::string& id,
    const std::string& filePath
)
{
    SurfacePtr surface{
        SDL_LoadBMP(filePath.c_str())
    };

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

    TexturePtr texture{
        SDL_CreateTextureFromSurface(
            renderer,
            surface.get()
        )
    };

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

    textures.insert_or_assign(
        id,
        std::move(texture)
    );

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
    const auto it = textures.find(id);

    if (it == textures.end())
    {
        return nullptr;
    }

    return it->second.get();
}

void TextureManager::Shutdown()
{
    textures.clear();
}
