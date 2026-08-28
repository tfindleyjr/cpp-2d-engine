#include "engine/arena/ArenaManager.h"
#include "engine/ecs/Registry.h"
#include "engine/physics/Physics.h"
#include "engine/world/Camera.h"

#include <cmath>
#include <iostream>
#include <string>

namespace
{
int failures = 0;

void Expect(bool condition, const std::string& message)
{
    if (!condition)
    {
        ++failures;
        std::cerr << "[FAIL] " << message << '\n';
    }
    else
    {
        std::cout << "[PASS] " << message << '\n';
    }
}

bool NearlyEqual(float a, float b, float epsilon = 0.001f)
{
    return std::abs(a - b) <= epsilon;
}

void TestPhysics()
{
    const TransformComponent aTransform{0.0f, 0.0f};
    const ColliderComponent aCollider{50.0f, 50.0f};

    const TransformComponent overlappingTransform{25.0f, 25.0f};
    const ColliderComponent overlappingCollider{50.0f, 50.0f};

    Expect(
        Physics::CheckCollision(
            aTransform,
            aCollider,
            overlappingTransform,
            overlappingCollider
        ),
        "AABB detects overlapping rectangles"
    );

    const TransformComponent separatedTransform{100.0f, 100.0f};

    Expect(
        !Physics::CheckCollision(
            aTransform,
            aCollider,
            separatedTransform,
            overlappingCollider
        ),
        "AABB rejects separated rectangles"
    );
}

void TestCamera()
{
    Camera camera(1280.0f, 720.0f);
    camera.SetWorldBounds(2400.0f, 1600.0f);

    camera.Follow(1175.0f, 775.0f, 50.0f, 50.0f);

    Expect(
        NearlyEqual(camera.GetX(), 560.0f) &&
        NearlyEqual(camera.GetY(), 440.0f),
        "Camera centers on a target inside the world"
    );

    camera.Follow(0.0f, 0.0f, 50.0f, 50.0f);

    Expect(
        NearlyEqual(camera.GetX(), 0.0f) &&
        NearlyEqual(camera.GetY(), 0.0f),
        "Camera clamps to the top-left world edge"
    );

    camera.Follow(2390.0f, 1590.0f, 10.0f, 10.0f);

    Expect(
        NearlyEqual(camera.GetX(), 1120.0f) &&
        NearlyEqual(camera.GetY(), 880.0f),
        "Camera clamps to the bottom-right world edge"
    );
}

void TestRegistry()
{
    Registry registry;

    const Entity first = registry.CreateEntity();
    const Entity second = registry.CreateEntity();

    registry.AddTransform(first, TransformComponent{10.0f, 20.0f});

    Expect(
        registry.IsAlive(first) && registry.IsAlive(second),
        "Registry tracks live entities"
    );

    TransformComponent* transform = registry.GetTransform(first);

    Expect(
        transform && NearlyEqual(transform->x, 10.0f) &&
        NearlyEqual(transform->y, 20.0f),
        "Registry stores and retrieves component data"
    );

    registry.DestroyEntity(first);

    Expect(
        !registry.IsAlive(first) && registry.GetTransform(first) == nullptr,
        "Destroying an entity removes its components"
    );

    const Entity recycled = registry.CreateEntity();

    Expect(
        recycled == first,
        "Registry recycles destroyed entity IDs"
    );
}

void TestArenaManager()
{
    ArenaManager arena;

    Expect(
        arena.GetWave() == 0 && arena.IsBetweenWaves(),
        "Arena begins between waves"
    );

    arena.Update(1.6f, 0);

    Expect(
        arena.GetWave() == 1 && !arena.IsBetweenWaves(),
        "Arena starts wave one after the opening countdown"
    );

    int spawned = 0;

    for (int i = 0; i < 100 && spawned < 5; ++i)
    {
        if (arena.ShouldSpawnEnemy())
        {
            ++spawned;
        }

        arena.Update(0.2f, static_cast<std::size_t>(spawned));
    }

    Expect(
        spawned == 5,
        "Wave one schedules the expected five enemies"
    );

    Expect(
        arena.GetEnemySpeed() > 105.0f && arena.GetEnemyHealth() >= 3,
        "Arena difficulty scaling is active"
    );
}
}

int main()
{
    TestPhysics();
    TestCamera();
    TestRegistry();
    TestArenaManager();

    if (failures == 0)
    {
        std::cout << "\nAll Arena Engine tests passed.\n";
        return 0;
    }

    std::cerr << "\n" << failures << " test(s) failed.\n";
    return 1;
}
