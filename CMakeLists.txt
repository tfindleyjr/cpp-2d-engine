cmake_minimum_required(VERSION 3.16)

project(ArenaEngine
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin"
)

add_subdirectory(
    vendored/SDL
    EXCLUDE_FROM_ALL
)

add_executable(
    ArenaGame

    game/main.cpp

    engine/core/Application.cpp
    engine/renderer/Renderer.cpp
    engine/input/Input.cpp
    engine/physics/Physics.cpp
    engine/resources/TextureManager.cpp
    engine/world/Camera.cpp
    engine/arena/ArenaManager.cpp
    engine/audio/AudioSystem.cpp
    engine/effects/GameFeel.cpp
    engine/debug/EngineDiagnostics.cpp
)

target_include_directories(
    ArenaGame
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ArenaGame
    PRIVATE
        SDL3::SDL3
)

if (MSVC)
    target_compile_options(
        ArenaGame
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(
        ArenaGame
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()
