#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
  public:
    Entity* entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}
};

class Entity
{
  public:
    Entity(Manager& manager): manager(manager) {}

    virtual void init() {}

    void update()
    {
        for (auto& c: components)
            c->update();
    }

    void draw()
    {
        for (auto& c: components)
            c->draw();
    }

    bool isActive() { return active; }
    void destroy() { active = false; }

    bool hasGroup(Group group) { return groupBitSet[group]; }

    void addGroup(Group group);
    void deleteGroup(Group group) { groupBitSet[group] = false; }

    template <typename T>
    bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... args)
    {
        T* component(new T(std::forward<TArgs>(args)...));
        component->entity = this;
        std::unique_ptr<Component> componentPtr(component);

        components.emplace_back(std::move(componentPtr));
        componentArray[getComponentTypeID<T>()] = component;
        componentBitSet[getComponentTypeID<T>()] = true;

        component->init();
        return *component;
    }

    // template <typename T, typename... TArgs>
    // std::shared_ptr<T> addComponent(TArgs&&... args)
    // {
    //     auto componentPtr = std::make_shared<T>(args...);
    //     componentPtr->entity = this;

    //     components.emplace_back(componentPtr.get());
    //     componentArray[getComponentTypeID<T>()] = componentPtr.get();
    //     componentBitSet[getComponentTypeID<T>()] = true;

    //     componentPtr->init();
    //     return componentPtr;
    // }

    template <typename T>
    T& getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }

  private:
    Manager& manager;
    bool active = true;
    std::vector<std::shared_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitSet;
};

class Manager
{
  private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;

  public:
    void update()
    {
        for (auto& e: entities)
            e->update();
    }

    void draw()
    {
        for (auto& e: entities)
            e->draw();
    }

    void refresh()
    {
        for (size_t i = 0; i < maxGroups; i++)
        {
            auto& v(groupedEntities[i]);
            v.erase(std::remove_if(
                        std::begin(v),
                        std::end(v),
                        [i](Entity* mEntity) { return !mEntity->isActive() || !mEntity->hasGroup(i); }),
                    std::end(v));
        }

        entities.erase(
            std::remove_if(std::begin(entities),
                           std::end(entities),
                           [](const std::shared_ptr<Entity> entity) { return !entity->isActive(); }),
            std::end(entities));
    }

    void addGroup(Entity* entity, Group group) { groupedEntities[group].emplace_back(entity); }

    std::vector<Entity*>& getGroup(Group group) { return groupedEntities[group]; }

    template <typename EntityClass = Entity>
    std::shared_ptr<EntityClass> addEntity()
    {
        auto entityPtr = std::make_shared<EntityClass>(*this);
        entityPtr->init();
        entities.emplace_back(entityPtr);
        return entityPtr;
    }
};
