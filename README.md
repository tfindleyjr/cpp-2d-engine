# Arena Engine

A C++20 / SDL3 2D game-engine project being developed into a top-down arena shooter.

## Current milestone: Phase 15

The project currently includes:

- SDL3 application/window lifecycle and delta-time game loop
- reusable renderer for primitives and textures
- keyboard and mouse input with held-key and one-shot key press handling
- lightweight entity-component registry
- templated `ComponentStore<T>` storage for reusable ECS component management
- recycled entity IDs and pre-reserved component storage to reduce allocation churn
- AABB collision detection
- texture loading and sprite animation support
- RAII-based SDL texture/surface ownership with `std::unique_ptr` and custom deleters
- camera tracking inside a 2400x1600 world
- world-to-screen coordinate conversion and optional scrolling grid
- mouse aiming and projectile combat
- projectile lifetime, obstacle collision, enemy damage, and score
- enemy chase AI, health, contact damage, and death
- wave-based arena progression with escalating enemy count, speed, health, and spawn rate
- procedural SDL audio feedback for shooting, hits, player damage, and wave starts
- screen shake, hit flash, damage flash, crosshair, and combat feedback
- HUD health bar, wave indicators, score/enemy information in the window title
- Playing, Paused, and Game Over states with restart support
- runtime developer diagnostics with FPS, average frame time, and live-entity counts
- toggleable collider visualization and world-grid debugging
- camera-aware render culling for bullets, enemies, and world geometry
- stricter cross-platform compiler warnings and generated `compile_commands.json`

## Controls

- `WASD` - move
- Mouse - aim
- Left mouse button - fire
- `P` - pause / resume
- `R` - restart after game over
- `F1` - toggle diagnostics in the window title
- `F2` - toggle collider visualization
- `F3` - toggle the world grid
- `Esc` - quit

## Arena loop

The arena begins with a short countdown and then automatically starts wave 1. Each completed wave creates a short break before the next wave. Later waves spawn more enemies and gradually increase enemy speed, health, and spawn frequency. Every defeated enemy awards 100 points.

## Phase 15 engine tools and polish

Phase 15 adds developer-facing runtime tools rather than another core gameplay mechanic. `EngineDiagnostics` samples frame timing every half-second and tracks FPS, average frame milliseconds, live ECS entities, bullets, and enemies. Pressing `F1` exposes the performance data in the window title while keeping the normal HUD uncluttered during play.

`F2` enables collider rendering for the player, obstacle, bullets, and enemies. This makes it possible to visually debug a common game-engine problem: the rendered object and the physics hitbox not lining up. `F3` hides or restores the scrolling world grid so the renderer can be inspected with less visual noise.

The renderer path now performs simple camera-aware culling before drawing world entities. Bullets, enemies, and world geometry that are fully outside the camera viewport are skipped instead of being submitted to SDL. This does not change simulation or collision logic; it only avoids unnecessary draw work.

## Build

```bash
cmake -S . -B build
cmake --build build
./build/bin/ArenaGame
```

For an optimized release build:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
./build-release/bin/ArenaGame
```

GitHub Codespaces can compile the project, but a normal desktop environment is required to display the SDL window. Audio is optional; if a playback device is unavailable, gameplay continues without sound.

## Optional player art

The game automatically falls back to a rectangle if `assets/player.bmp` is missing. To use the sprite-animation path, provide a horizontal four-frame BMP sprite sheet with 32x32 frames (128x32 total).

## Architecture

```text
Application
├── Input
├── Renderer
├── TextureManager (RAII resource ownership)
├── Camera
├── ArenaManager
├── AudioSystem
├── GameFeel
├── EngineDiagnostics
│   ├── FPS / frame-time sampling
│   ├── live-entity counters
│   ├── collider visualization
│   └── grid toggle
├── Registry
│   ├── ComponentStore<T>
│   ├── entity lifetime tracking
│   └── recycled entity IDs
└── Physics

Gameplay
├── Player
├── Projectiles
├── Enemies
├── Wave progression
├── Score / health HUD
└── Playing / Paused / Game Over states
```

The final milestone is Phase 16: testing, release packaging, documentation, and portfolio presentation.
