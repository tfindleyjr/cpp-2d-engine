#pragma once

#include <SDL3/SDL.h>

class Input
{
public:
    Input();

    void Update();

    bool ShouldQuit() const;

    bool IsKeyDown(SDL_Scancode key) const;

private:
    bool quitRequested;
};