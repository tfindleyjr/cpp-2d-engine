cmake_minimum_required(VERSION 3.16)

project(ArenaEngine
    VERSION 1.0.0
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

function(enable_strict_warnings target)
    if (MSVC)
        target_compile_options(
            ${target}
            PRIVATE
                /W4
                /permissive-
        )
    else()
        target_compile_options(
            ${target}
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()
endfunction()

enable_strict_warnings(ArenaGame)

include(CTest)

if (BUILD_TESTING)
    add_executable(
        ArenaTests

        tests/TestMain.cpp
        engine/physics/Physics.cpp
        engine/world/Camera.cpp
        engine/arena/ArenaManager.cpp
    )

    target_include_directories(
        ArenaTests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    enable_strict_warnings(ArenaTests)

    add_test(
        NAME ArenaEngineTests
        COMMAND ArenaTests
    )
endif()

install(
    TARGETS ArenaGame
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY assets/
    DESTINATION assets
)

set(CPACK_PACKAGE_NAME "ArenaEngine")
set(CPACK_PACKAGE_VENDOR "Trenton Findley Jr.")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++20 SDL3 top-down arena shooter and custom 2D engine")
set(CPACK_GENERATOR "ZIP")
include(CPack)
