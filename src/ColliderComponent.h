#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "WindowManager.h"

#include <SDL2/SDL_rect.h>
#include <string>

class ColliderComponent: public Component
{
  public:
    SDL_FRect collider;
    std::string tag;

    TransformComponent* transform;

    ColliderComponent() = default;
    ColliderComponent(std::string tag): tag(tag) {}

    void init() override
    {
        LOG_IF(ERROR, !entity->hasComponent<TransformComponent>()) << "Missing transform component";
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        collider.x = transform->point2.x - WindowManager::Instance()->camera.x;
        collider.y = transform->point2.y - WindowManager::Instance()->camera.y;
        collider.w = transform->size2.w;
        collider.h = transform->size2.h;
    }

#ifdef DEBUG
    void draw() override
    {
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0xff, 0, 0, 0);
        SDL_RenderDrawRectF(WindowManager::Instance()->renderer, &collider);
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 0, 0);
    }
#endif
};

class Collision
{
  public:
    static bool AABB(const SDL_FRect a, SDL_FRect b) { return SDL_HasRectIntersectionFloat(&a, &b) == SDL_TRUE; }
    static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB)
    {
        return AABB(colA.collider, colB.collider);
    }
};
