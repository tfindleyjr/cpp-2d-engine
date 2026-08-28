# Arena Engine — Portfolio Notes

## One-line description

A C++20 / SDL3 custom 2D engine and top-down arena shooter featuring an ECS-style architecture, camera system, collision detection, wave-based combat, audio/game-feel systems, diagnostics, testing, and CI.

## Resume bullets

- Built a custom 2D game engine in C++20 with SDL3, implementing a delta-time game loop, rendering, keyboard/mouse input, AABB collision detection, camera tracking, resource management, and sprite animation.
- Designed an ECS-style architecture with templated component storage, entity lifetime tracking, recycled entity IDs, and RAII-managed SDL resources using smart pointers and custom deleters.
- Developed a playable arena-shooter loop with mouse aiming, projectiles, enemy steering AI, health/damage, wave scaling, procedural audio, screen shake, game states, and runtime debugging tools.
- Added CTest unit coverage, GitHub Actions CI, strict compiler warnings, release builds, render culling, performance diagnostics, and CPack packaging for a production-style development workflow.

## Interview explanation

I started the project as a minimal SDL3 application and intentionally built the engine in layers rather than writing the game as one monolithic file. The core loop separates input, simulation, and rendering. Game objects use an ECS-style registry where entities are IDs and components hold data such as transforms, velocity, health, collision boxes, projectiles, and enemy behavior.

As the project grew, I added a larger world with a camera, mouse-directed shooting, enemy steering, wave progression, audio feedback, screen shake, pause/game-over states, and debug tooling. I later refactored the ECS with a templated component store, added recycled entity IDs and RAII ownership for SDL resources, then added render culling, diagnostics, unit tests, CI, and release packaging.

The project taught me how architecture decisions affect maintainability. For example, separating simulation from rendering made camera transforms and culling much easier, while component-based entities allowed bullets and enemies to share engine systems without requiring a deep inheritance hierarchy.

## Technical topics to be ready to explain

- Why delta time makes movement frame-rate independent.
- How AABB collision detection works.
- Why vectors are normalized for movement, aiming, and enemy steering.
- The difference between world coordinates and screen coordinates.
- Entity IDs versus component data in an ECS-style design.
- Why `ComponentStore<T>` uses templates.
- What RAII means and why `std::unique_ptr` custom deleters are useful for SDL resources.
- Why off-screen entities can continue simulation while being culled from rendering.
- How one-shot key input differs from held-key input.
- How unit tests and CI reduce regressions.

## Suggested portfolio card

**Arena Engine — C++20 / SDL3**

Custom 2D game engine developed into a playable top-down arena shooter. Features ECS-style entity management, frame-independent movement, AABB physics, camera/world coordinates, mouse-directed combat, enemy AI, escalating waves, procedural audio, game-feel effects, runtime diagnostics, unit tests, CI, and release packaging.

**Tech:** C++20, SDL3, CMake, CTest, GitHub Actions, Git

## Demo checklist

For a portfolio video or GIF, show movement and camera scrolling first, then mouse aiming and shooting, enemy waves and health bars, pause/restart states, and finally toggle F1/F2/F3 to show the engine diagnostics, collision boxes, and grid controls.
