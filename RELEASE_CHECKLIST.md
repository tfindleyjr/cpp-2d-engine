# Arena Engine v1.0.0 Release Checklist

Use this checklist before creating a public GitHub release or recording a portfolio demo.

## Verification

- [ ] `git pull origin main` completes cleanly.
- [ ] `git submodule update --init --recursive` downloads SDL.
- [ ] Debug build completes without warnings from Arena Engine source.
- [ ] `ctest --test-dir build --output-on-failure` passes.
- [ ] Release build completes.
- [ ] `ctest --test-dir build-release --output-on-failure` passes.
- [ ] GitHub Actions CI is green.

## Gameplay smoke test

- [ ] Game launches on a desktop environment.
- [ ] WASD movement works in all four directions.
- [ ] Camera follows the player and clamps at world edges.
- [ ] Mouse aiming and left-click shooting work.
- [ ] Bullets collide with enemies and the world obstacle.
- [ ] Enemy health and player health update correctly.
- [ ] Waves progress and scale difficulty.
- [ ] Pause/resume works with `P`.
- [ ] Game Over and restart work with `R`.
- [ ] Audio failure does not prevent gameplay.
- [ ] `F1` diagnostics, `F2` colliders, and `F3` grid toggles work.

## Portfolio capture

- [ ] Record a 20–45 second gameplay clip.
- [ ] Show camera movement and world scrolling.
- [ ] Show shooting and enemy waves.
- [ ] Show health bars, score, pause, and restart.
- [ ] Toggle diagnostics and collider rendering for a few seconds.
- [ ] Add the GitHub repository to the portfolio project card.
- [ ] Use the resume bullets in `PORTFOLIO.md` as a starting point.

## Release package

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build-release
ctest --test-dir build-release --output-on-failure
cd build-release
cpack
```

Attach the generated ZIP to the GitHub v1.0.0 release after performing the gameplay smoke test on a normal desktop environment.
