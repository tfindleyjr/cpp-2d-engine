#pragma once

#include <cstddef>
#include <unordered_map>

#include "Entity.h"

template <typename Component>
class ComponentStore
{
public:
    void Reserve(std::size_t capacity)
    {
        components.reserve(capacity);
    }

    void Add(Entity entity, const Component& component)
    {
        components.insert_or_assign(entity, component);
    }

    Component* Get(Entity entity)
    {
        auto it = components.find(entity);
        return it == components.end() ? nullptr : &it->second;
    }

    const Component* Get(Entity entity) const
    {
        auto it = components.find(entity);
        return it == components.end() ? nullptr : &it->second;
    }

    void Remove(Entity entity)
    {
        components.erase(entity);
    }

    std::size_t Size() const
    {
        return components.size();
    }

private:
    std::unordered_map<Entity, Component> components;
};
