#include "Physics.h"

bool Physics::CheckCollision(
    const TransformComponent& aTransform,
    const ColliderComponent& aCollider,

    const TransformComponent& bTransform,
    const ColliderComponent& bCollider
)
{
    bool collisionX =
        aTransform.x <
            bTransform.x + bCollider.width
        &&
        aTransform.x + aCollider.width >
            bTransform.x;

    bool collisionY =
        aTransform.y <
            bTransform.y + bCollider.height
        &&
        aTransform.y + aCollider.height >
            bTransform.y;

    return collisionX && collisionY;
}