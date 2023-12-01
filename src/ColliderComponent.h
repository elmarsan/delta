// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "ECS.h"
#include "System.h"
#include "TransformComponent.h"

#include <string>

class ColliderComponent: public Component
{
  public:
    Rect collider;
    // SDL_FRect collider;
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
        auto camPos = System::actorSystem().getCameraPos();
        collider.x = transform->point2.x - camPos.x;
        collider.y = transform->point2.y - camPos.y;
        collider.w = transform->size2.w;
        collider.h = transform->size2.h;
    }

#ifdef DEBUG
    void draw() override
    {
        System::windowSystem().renderRect(collider);
        // SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0xff, 0, 0, 0);
        // SDL_RenderDrawRectF(WindowManager::Instance()->renderer, &collider);
        // SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 0, 0);
    }
#endif
};

// TODO: Add physics system
// class Collision
// {
//   public:
//     static bool AABB(const SDL_FRect a, SDL_FRect b) { return SDL_HasIntersectionF(&a, &b) == SDL_TRUE; }
//     static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB)
//     {
//         return AABB(colA.collider, colB.collider);
//     }
// };
