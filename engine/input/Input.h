#pragma once

#include <SDL3/SDL.h>

#include <unordered_set>

class Input
{
public:
    Input();

    void Update();

    bool ShouldQuit() const;
    bool IsKeyDown(SDL_Scancode key) const;
    bool WasKeyPressed(SDL_Scancode key) const;
    bool IsMouseButtonDown(Uint32 buttonMask) const;

    void GetMousePosition(
        float& x,
        float& y
    ) const;

private:
    bool quitRequested;
    std::unordered_set<SDL_Scancode> pressedKeys;
};
