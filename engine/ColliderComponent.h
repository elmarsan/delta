#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "delta/Game.h"

#include <SDL.h>
#include <string>

class ColliderComponent: public Component
{
  public:
    SDL_Rect collider;
    std::string tag;

    TransformComponent* transform;

    ColliderComponent(std::string tag): tag(tag) {}

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        collider.x = transform->position.x;
        collider.y = transform->position.y;
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }

#ifdef DEBUG
    void draw() override
    {
        SDL_SetRenderDrawColor(Game::renderer, 0xff, 0, 0, 0);
        SDL_RenderDrawRect(Game::renderer, &collider);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
    }
#endif
};

class Collision
{
  public:
    static bool AABB(const SDL_Rect a, SDL_Rect b) { return SDL_HasIntersection(&a, &b) == SDL_TRUE; }
    static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB)
    {
        return AABB(colA.collider, colB.collider);
    }
};
