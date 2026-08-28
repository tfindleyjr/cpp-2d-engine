# Arena Engine

![CI](https://github.com/tfindleyjr/cpp-2d-engine/actions/workflows/ci.yml/badge.svg)

A C++20 / SDL3 custom 2D engine developed into a playable top-down arena shooter.

## Status

**Phase 16 complete — v1.0.0 portfolio release.**

The project includes:

- SDL3 application/window lifecycle and delta-time game loop
- reusable renderer for primitives and textures
- keyboard and mouse input with held-key and one-shot key press handling
- ECS-style entity/component registry
- templated `ComponentStore<T>` storage
- recycled entity IDs and pre-reserved component storage
- AABB collision detection
- texture loading and sprite animation support
- RAII-based SDL texture/surface ownership with `std::unique_ptr` custom deleters
- 2400x1600 world with camera tracking and world-to-screen conversion
- mouse aiming and projectile combat
- enemy steering AI, health, contact damage, and death
- wave-based arena progression with scaling enemy count, speed, health, and spawn rate
- procedural SDL audio for shooting, hits, player damage, and wave starts
- screen shake, hit flash, damage flash, crosshair, and combat feedback
- Playing, Paused, and Game Over states with restart support
- runtime diagnostics for FPS, frame time, and live entity counts
- toggleable collider visualization and world-grid debugging
- camera-aware render culling
- strict compiler warnings and `compile_commands.json`
- CTest unit tests for physics, camera behavior, entity lifecycle, and arena progression
- GitHub Actions continuous integration
- CPack ZIP release packaging

## Controls

- `WASD` — move
- Mouse — aim
- Left mouse button — fire
- `P` — pause / resume
- `R` — restart after game over
- `F1` — toggle diagnostics in the window title
- `F2` — toggle collider visualization
- `F3` — toggle the world grid
- `Esc` — quit

## Arena loop

The arena begins with a short countdown and automatically starts wave 1. Each completed wave creates a short break before the next wave. Later waves spawn more enemies and gradually increase enemy speed, health, and spawn frequency. Every defeated enemy awards 100 points.

## Build

Clone with submodules so SDL is available:

```bash
git clone --recurse-submodules https://github.com/tfindleyjr/cpp-2d-engine.git
cd cpp-2d-engine
```

Configure and build:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
```

Run the game from the project root:

```bash
./build/bin/ArenaGame
```

GitHub Codespaces can compile and test the project, but a normal desktop environment is required to display the SDL window. Audio is optional; if no playback device is available, gameplay continues without sound.

## Run tests

```bash
ctest --test-dir build --output-on-failure
```

The Phase 16 test executable covers core engine logic that can run without opening a game window:

- AABB overlap and separation
- camera centering and world-edge clamping
- registry component access, destruction, and entity-ID recycling
- wave startup, spawn scheduling, and difficulty scaling

## Release build

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build-release
ctest --test-dir build-release --output-on-failure
```

Create a ZIP package with CPack:

```bash
cd build-release
cpack
```

This produces an `ArenaEngine-1.0.0-*.zip` package containing the installable game target and assets directory.

## Continuous integration

`.github/workflows/ci.yml` automatically configures a Release build and runs the test suite for pushes to `main` and for pull requests. This provides a repeatable build/test check independent of the developer machine.

## Optional player art

The game falls back to a rectangle if `assets/player.bmp` is missing. To use the sprite-animation path, provide a horizontal four-frame BMP sprite sheet with 32x32 frames, 128x32 total.

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

Quality / Delivery
├── CTest unit tests
├── GitHub Actions CI
├── strict compiler warnings
├── Release configuration
└── CPack ZIP packaging
```

## Portfolio / interview notes

See [`PORTFOLIO.md`](PORTFOLIO.md) for concise resume bullets, an interview explanation, technical talking points, and a suggested demo sequence.

## Project progression

The engine was intentionally built in 16 phases: project setup, game loop, rendering, input, ECS, physics, assets/animation, camera/world, combat, enemy AI, waves, audio/game feel, UI/game states, modern C++ optimization, developer tooling, and finally testing/release packaging.
