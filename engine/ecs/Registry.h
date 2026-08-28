#pragma once

#include <unordered_map>

#include "Entity.h"

#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/ColliderComponent.h"
#include "components/SpriteComponent.h"
#include "components/AnimationComponent.h"
#include "components/ProjectileComponent.h"
#include "components/HealthComponent.h"
#include "components/EnemyComponent.h"

class Registry
{
public:
    Entity CreateEntity()
    {
        return nextEntity++;
    }

    void DestroyEntity(Entity entity)
    {
        transforms.erase(entity);
        velocities.erase(entity);
        colliders.erase(entity);
        sprites.erase(entity);
        animations.erase(entity);
        projectiles.erase(entity);
        health.erase(entity);
        enemies.erase(entity);
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

    void AddProjectile(
        Entity entity,
        const ProjectileComponent& projectile
    )
    {
        projectiles[entity] = projectile;
    }

    void AddHealth(
        Entity entity,
        const HealthComponent& healthComponent
    )
    {
        health[entity] = healthComponent;
    }

    void AddEnemy(
        Entity entity,
        const EnemyComponent& enemy
    )
    {
        enemies[entity] = enemy;
    }

    TransformComponent* GetTransform(Entity entity)
    {
        auto it = transforms.find(entity);
        return it == transforms.end() ? nullptr : &it->second;
    }

    VelocityComponent* GetVelocity(Entity entity)
    {
        auto it = velocities.find(entity);
        return it == velocities.end() ? nullptr : &it->second;
    }

    ColliderComponent* GetCollider(Entity entity)
    {
        auto it = colliders.find(entity);
        return it == colliders.end() ? nullptr : &it->second;
    }

    SpriteComponent* GetSprite(Entity entity)
    {
        auto it = sprites.find(entity);
        return it == sprites.end() ? nullptr : &it->second;
    }

    AnimationComponent* GetAnimation(Entity entity)
    {
        auto it = animations.find(entity);
        return it == animations.end() ? nullptr : &it->second;
    }

    ProjectileComponent* GetProjectile(Entity entity)
    {
        auto it = projectiles.find(entity);
        return it == projectiles.end() ? nullptr : &it->second;
    }

    HealthComponent* GetHealth(Entity entity)
    {
        auto it = health.find(entity);
        return it == health.end() ? nullptr : &it->second;
    }

    EnemyComponent* GetEnemy(Entity entity)
    {
        auto it = enemies.find(entity);
        return it == enemies.end() ? nullptr : &it->second;
    }

private:
    Entity nextEntity = 0;

    std::unordered_map<Entity, TransformComponent> transforms;
    std::unordered_map<Entity, VelocityComponent> velocities;
    std::unordered_map<Entity, ColliderComponent> colliders;
    std::unordered_map<Entity, SpriteComponent> sprites;
    std::unordered_map<Entity, AnimationComponent> animations;
    std::unordered_map<Entity, ProjectileComponent> projectiles;
    std::unordered_map<Entity, HealthComponent> health;
    std::unordered_map<Entity, EnemyComponent> enemies;
};
