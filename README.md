# Arena Engine

A C++20 / SDL3 2D game-engine project being developed into a top-down arena shooter.

## Current milestone: Phase 13

The project currently includes:

- SDL3 application/window lifecycle and delta-time game loop
- reusable renderer for primitives and textures
- keyboard and mouse input with held-key and one-shot key press handling
- lightweight entity-component registry
- AABB collision detection
- texture loading and sprite animation support
- camera tracking inside a 2400x1600 world
- world-to-screen coordinate conversion and scrolling grid
- mouse aiming and projectile combat
- projectile lifetime, obstacle collision, enemy damage, and score
- enemy chase AI, health, contact damage, and death
- wave-based arena progression with escalating enemy count, speed, health, and spawn rate
- procedural SDL audio feedback for shooting, hits, player damage, and wave starts
- screen shake, hit flash, damage flash, crosshair, and combat feedback
- HUD health bar, wave indicators, score/enemy information in the window title
- Playing, Paused, and Game Over states with restart support

## Controls

- `WASD` - move
- Mouse - aim
- Left mouse button - fire
- `P` - pause / resume
- `R` - restart after game over
- `Esc` - quit

## Arena loop

The arena begins with a short countdown and then automatically starts wave 1. Each completed wave creates a short break before the next wave. Later waves spawn more enemies and gradually increase enemy speed, health, and spawn frequency. Every defeated enemy awards 100 points.

## Build

```bash
cmake -S . -B build
cmake --build build
./build/bin/ArenaGame
```

GitHub Codespaces can compile the project, but a normal desktop environment is required to display the SDL window. Audio is optional; if a playback device is unavailable, gameplay continues without sound.

## Optional player art

The game automatically falls back to a rectangle if `assets/player.bmp` is missing. To use the sprite-animation path, provide a horizontal four-frame BMP sprite sheet with 32x32 frames (128x32 total).

## Architecture

```text
Application
├── Input
├── Renderer
├── TextureManager
├── Camera
├── ArenaManager
├── AudioSystem
├── GameFeel
├── Registry / ECS Components
└── Physics

Gameplay
├── Player
├── Projectiles
├── Enemies
├── Wave progression
├── Score / health HUD
└── Playing / Paused / Game Over states
```

The next milestone is Phase 14: advanced C++ architecture and optimization.
