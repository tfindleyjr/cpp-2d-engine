#pragma once

#include "engine/ecs/components/TransformComponent.h"
#include "engine/ecs/components/ColliderComponent.h"

class Physics
{
public:
    static bool CheckCollision(
        const TransformComponent& aTransform,
        const ColliderComponent& aCollider,

        const TransformComponent& bTransform,
        const ColliderComponent& bCollider
    );
};