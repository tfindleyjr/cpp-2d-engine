#include "Renderer.h"

#include <iostream>

Renderer::Renderer()
    : renderer(nullptr)
{
}

Renderer::~Renderer()
{
    Shutdown();
}

bool Renderer::Initialize(
    SDL_Window* window
)
{
    renderer =
        SDL_CreateRenderer(
            window,
            nullptr
        );

    if (!renderer)
    {
        std::cerr
            << "Renderer creation failed: "
            << SDL_GetError()
            << std::endl;

        return false;
    }

    std::cout
        << "Renderer initialized successfully."
        << std::endl;

    return true;
}

void Renderer::Shutdown()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}

void Renderer::BeginFrame()
{
    SDL_SetRenderDrawColor(
        renderer,
        20,
        20,
        30,
        255
    );

    SDL_RenderClear(renderer);
}

void Renderer::EndFrame()
{
    SDL_RenderPresent(renderer);
}

void Renderer::SetDrawColor(
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a
)
{
    SDL_SetRenderDrawColor(
        renderer,
        r,
        g,
        b,
        a
    );
}

void Renderer::DrawRectangle(
    float x,
    float y,
    float width,
    float height
)
{
    SDL_FRect rectangle{
        x,
        y,
        width,
        height
    };

    SDL_RenderRect(
        renderer,
        &rectangle
    );
}

void Renderer::FillRectangle(
    float x,
    float y,
    float width,
    float height
)
{
    SDL_FRect rectangle{
        x,
        y,
        width,
        height
    };

    SDL_RenderFillRect(
        renderer,
        &rectangle
    );
}

void Renderer::DrawTexture(
    SDL_Texture* texture,
    const SDL_FRect* source,
    const SDL_FRect* destination
)
{
    if (!texture)
    {
        return;
    }

    SDL_RenderTexture(
        renderer,
        texture,
        source,
        destination
    );
}

SDL_Renderer*
Renderer::GetSDLRenderer()
{
    return renderer;
}