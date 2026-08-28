#pragma once

#include <SDL3/SDL.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize(SDL_Window* window);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    void SetDrawColor(
        Uint8 r,
        Uint8 g,
        Uint8 b,
        Uint8 a = 255
    );

    void DrawRectangle(
        float x,
        float y,
        float width,
        float height
    );

    void FillRectangle(
        float x,
        float y,
        float width,
        float height
    );

    void DrawTexture(
        SDL_Texture* texture,
        const SDL_FRect* source,
        const SDL_FRect* destination
    );

    SDL_Renderer* GetSDLRenderer();

private:
    SDL_Renderer* renderer;
};