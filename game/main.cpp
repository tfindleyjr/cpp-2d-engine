#include "engine/core/Application.h"

#include <iostream>

int main()
{
    Application app;

    if (!app.Initialize())
    {
        std::cerr << "Failed to initialize application." << std::endl;
        return 1;
    }

    app.Run();

    return 0;
}