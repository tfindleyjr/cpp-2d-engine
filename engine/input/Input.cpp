#include "Input.h"

Input::Input()
    : quitRequested(false)
{
}

void Input::Update()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            quitRequested = true;
        }
    }
}

bool Input::ShouldQuit() const
{
    return quitRequested;
}

bool Input::IsKeyDown(SDL_Scancode key) const
{
    const bool* keyboardState =
        SDL_GetKeyboardState(nullptr);

    return keyboardState[key];
}