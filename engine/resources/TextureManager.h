#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <string>
#include <unordered_map>

struct SDLTextureDeleter
{
    void operator()(SDL_Texture* texture) const
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }
};

using TexturePtr = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

class TextureManager
{
public:
    TextureManager() = default;
    ~TextureManager() = default;

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager(TextureManager&&) noexcept = default;
    TextureManager& operator=(TextureManager&&) noexcept = default;

    bool LoadTexture(
        SDL_Renderer* renderer,
        const std::string& id,
        const std::string& filePath
    );

    SDL_Texture* GetTexture(const std::string& id);

    void Shutdown();

private:
    std::unordered_map<std::string, TexturePtr> textures;
};
