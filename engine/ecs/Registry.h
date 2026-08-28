#pragma once

#include <unordered_map>

#include "Entity.h"

#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/ColliderComponent.h"
#include "components/SpriteComponent.h"
#include "components/AnimationComponent.h"

class Registry
{
public:
    Entity CreateEntity()
    {
        return nextEntity++;
    }

    void AddTransform(
        Entity entity,
        const TransformComponent& transform
    )
    {
        transforms[entity] = transform;
    }

    void AddVelocity(
        Entity entity,
        const VelocityComponent& velocity
    )
    {
        velocities[entity] = velocity;
    }

    void AddCollider(
        Entity entity,
        const ColliderComponent& collider
    )
    {
        colliders[entity] = collider;
    }

    void AddSprite(
        Entity entity,
        const SpriteComponent& sprite
    )
    {
        sprites[entity] = sprite;
    }

    void AddAnimation(
        Entity entity,
        const AnimationComponent& animation
    )
    {
        animations[entity] = animation;
    }

    TransformComponent*
    GetTransform(Entity entity)
    {
        auto it =
            transforms.find(entity);

        if (it == transforms.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    VelocityComponent*
    GetVelocity(Entity entity)
    {
        auto it =
            velocities.find(entity);

        if (it == velocities.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    ColliderComponent*
    GetCollider(Entity entity)
    {
        auto it =
            colliders.find(entity);

        if (it == colliders.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    SpriteComponent*
    GetSprite(Entity entity)
    {
        auto it =
            sprites.find(entity);

        if (it == sprites.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    AnimationComponent*
    GetAnimation(Entity entity)
    {
        auto it =
            animations.find(entity);

        if (it == animations.end())
        {
            return nullptr;
        }

        return &it->second;
    }

private:
    Entity nextEntity = 0;

    std::unordered_map<
        Entity,
        TransformComponent
    > transforms;

    std::unordered_map<
        Entity,
        VelocityComponent
    > velocities;

    std::unordered_map<
        Entity,
        ColliderComponent
    > colliders;

    std::unordered_map<
        Entity,
        SpriteComponent
    > sprites;

    std::unordered_map<
        Entity,
        AnimationComponent
    > animations;
};