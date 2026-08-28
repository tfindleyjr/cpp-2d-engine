#pragma once

#include <unordered_map>

#include "Entity.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

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

    TransformComponent* GetTransform(Entity entity)
    {
        auto it = transforms.find(entity);

        if (it == transforms.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    VelocityComponent* GetVelocity(Entity entity)
    {
        auto it = velocities.find(entity);

        if (it == velocities.end())
        {
            return nullptr;
        }

        return &it->second;
    }

private:
    Entity nextEntity = 0;

    std::unordered_map<Entity, TransformComponent>
        transforms;

    std::unordered_map<Entity, VelocityComponent>
        velocities;
};