#include "Input.h"

Input::Input()
    : quitRequested(false)
{
}

void Input::Update()
{
    pressedKeys.clear();

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            quitRequested = true;
        }
        else if (
            event.type == SDL_EVENT_KEY_DOWN &&
            !event.key.repeat
        )
        {
            pressedKeys.insert(event.key.scancode);
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

bool Input::WasKeyPressed(SDL_Scancode key) const
{
    return pressedKeys.contains(key);
}

bool Input::IsMouseButtonDown(Uint32 buttonMask) const
{
    float mouseX = 0.0f;
    float mouseY = 0.0f;

    const Uint32 buttons = static_cast<Uint32>(
        SDL_GetMouseState(&mouseX, &mouseY)
    );

    return (buttons & buttonMask) != 0;
}

void Input::GetMousePosition(
    float& x,
    float& y
) const
{
    SDL_GetMouseState(&x, &y);
}
