# Arena Engine

A C++20 / SDL3 2D game-engine project being developed into a top-down arena shooter.

## Current milestone: Phase 10

The project currently includes:

- SDL3 application/window lifecycle and delta-time game loop
- reusable renderer for primitives and textures
- keyboard and mouse input
- lightweight entity-component registry
- AABB collision detection
- texture loading and sprite animation support
- camera tracking inside a 2400x1600 world
- world-to-screen coordinate conversion and scrolling grid
- mouse aiming and projectile combat
- projectile lifetime, obstacle collision, and enemy damage
- enemy chase AI, health, contact damage, and death
- simple player/enemy health bars

## Controls

- `WASD` - move
- Mouse - aim
- Left mouse button - fire
- `Esc` - quit

## Build

```bash
cmake -S . -B build
cmake --build build
./build/bin/ArenaGame
```

GitHub Codespaces can compile the project, but a normal desktop environment is required to display the SDL window.

## Optional player art

The game automatically falls back to a rectangle if `assets/player.bmp` is missing. To use the Phase 7 animation path, provide a horizontal four-frame BMP sprite sheet with 32x32 frames (128x32 total).

## Architecture

```text
Application
├── Input
├── Renderer
├── TextureManager
├── Camera
├── Registry / ECS Components
└── Physics

Gameplay
├── Player
├── Projectiles
├── Enemies
└── World obstacle
```

The next milestone is Phase 11: arena and wave management.
