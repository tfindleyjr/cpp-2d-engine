#pragma once

#include <cstddef>
#include <unordered_set>
#include <vector>

#include "ComponentStore.h"
#include "Entity.h"

#include "components/AnimationComponent.h"
#include "components/ColliderComponent.h"
#include "components/EnemyComponent.h"
#include "components/HealthComponent.h"
#include "components/ProjectileComponent.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

class Registry
{
public:
    Registry()
    {
        constexpr std::size_t initialCapacity = 256;

        aliveEntities.reserve(initialCapacity);
        freeEntities.reserve(initialCapacity);
        transforms.Reserve(initialCapacity);
        velocities.Reserve(initialCapacity);
        colliders.Reserve(initialCapacity);
        sprites.Reserve(initialCapacity);
        animations.Reserve(initialCapacity);
        projectiles.Reserve(initialCapacity);
        health.Reserve(initialCapacity);
        enemies.Reserve(initialCapacity);
    }

    Entity CreateEntity()
    {
        Entity entity = 0;

        if (!freeEntities.empty())
        {
            entity = freeEntities.back();
            freeEntities.pop_back();
        }
        else
        {
            entity = nextEntity++;
        }

        aliveEntities.insert(entity);
        return entity;
    }

    void DestroyEntity(Entity entity)
    {
        if (aliveEntities.erase(entity) == 0)
        {
            return;
        }

        transforms.Remove(entity);
        velocities.Remove(entity);
        colliders.Remove(entity);
        sprites.Remove(entity);
        animations.Remove(entity);
        projectiles.Remove(entity);
        health.Remove(entity);
        enemies.Remove(entity);

        freeEntities.push_back(entity);
    }

    bool IsAlive(Entity entity) const
    {
        return aliveEntities.contains(entity);
    }

    std::size_t AliveCount() const
    {
        return aliveEntities.size();
    }

    void AddTransform(Entity entity, const TransformComponent& component)
    {
        transforms.Add(entity, component);
    }

    void AddVelocity(Entity entity, const VelocityComponent& component)
    {
        velocities.Add(entity, component);
    }

    void AddCollider(Entity entity, const ColliderComponent& component)
    {
        colliders.Add(entity, component);
    }

    void AddSprite(Entity entity, const SpriteComponent& component)
    {
        sprites.Add(entity, component);
    }

    void AddAnimation(Entity entity, const AnimationComponent& component)
    {
        animations.Add(entity, component);
    }

    void AddProjectile(Entity entity, const ProjectileComponent& component)
    {
        projectiles.Add(entity, component);
    }

    void AddHealth(Entity entity, const HealthComponent& component)
    {
        health.Add(entity, component);
    }

    void AddEnemy(Entity entity, const EnemyComponent& component)
    {
        enemies.Add(entity, component);
    }

    TransformComponent* GetTransform(Entity entity)
    {
        return transforms.Get(entity);
    }

    VelocityComponent* GetVelocity(Entity entity)
    {
        return velocities.Get(entity);
    }

    ColliderComponent* GetCollider(Entity entity)
    {
        return colliders.Get(entity);
    }

    SpriteComponent* GetSprite(Entity entity)
    {
        return sprites.Get(entity);
    }

    AnimationComponent* GetAnimation(Entity entity)
    {
        return animations.Get(entity);
    }

    ProjectileComponent* GetProjectile(Entity entity)
    {
        return projectiles.Get(entity);
    }

    HealthComponent* GetHealth(Entity entity)
    {
        return health.Get(entity);
    }

    EnemyComponent* GetEnemy(Entity entity)
    {
        return enemies.Get(entity);
    }

private:
    Entity nextEntity = 0;

    std::unordered_set<Entity> aliveEntities;
    std::vector<Entity> freeEntities;

    ComponentStore<TransformComponent> transforms;
    ComponentStore<VelocityComponent> velocities;
    ComponentStore<ColliderComponent> colliders;
    ComponentStore<SpriteComponent> sprites;
    ComponentStore<AnimationComponent> animations;
    ComponentStore<ProjectileComponent> projectiles;
    ComponentStore<HealthComponent> health;
    ComponentStore<EnemyComponent> enemies;
};
