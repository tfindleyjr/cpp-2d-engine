# Changelog

## v1.0.0 — Arena Engine portfolio release

### Engine

- SDL3 application lifecycle and delta-time game loop.
- Primitive and texture rendering.
- Keyboard and mouse input with held and one-shot input states.
- ECS-style registry with templated component stores.
- Entity destruction, live-entity tracking, and recycled entity IDs.
- AABB collision detection.
- Camera tracking and world-to-screen coordinate conversion.
- RAII ownership for SDL textures and surfaces.
- Runtime diagnostics and camera-aware render culling.

### Gameplay

- WASD movement with normalized diagonal speed.
- Mouse-directed projectile combat.
- Enemy chase steering and contact damage.
- Health, death, score, and escalating wave progression.
- Procedural SDL sound effects.
- Screen shake, hit flash, damage flash, and crosshair feedback.
- Playing, Paused, and Game Over states with restart support.

### Developer workflow

- Strict compiler warnings for GCC, Clang, and MSVC.
- Generated `compile_commands.json`.
- CTest-based engine unit tests.
- GitHub Actions continuous integration.
- CPack ZIP release packaging.
- Portfolio and release documentation.
