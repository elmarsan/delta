#pragma once

#include "ECS.h"
#include "SDL.h"
#include "TransformComponent.h"

#include <string>

namespace engine
{

namespace component
{
    class Collider: public Component
    {
      public:
        SDL_Rect collider;
        std::string tag;

        Transform* transform;

        Collider(std::string tag): tag(tag) {}

        void init() override
        {
            if (!entity->hasComponent<Transform>())
            {
                entity->addComponent<Transform>();
            }
            transform = &entity->getComponent<Transform>();

            // delta::Game::colliders.push_back(this);
        }

        void update() override
        {
            collider.x = transform->position.x;
            collider.y = transform->position.y;
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        }

        void draw() override
        {
            // SDL_SetRenderDrawColor(delta::Game::renderer, 0xff, 0, 0, 0);
            // SDL_RenderDrawRect(delta::Game::renderer, &collider);
            // SDL_SetRenderDrawColor(delta::Game::renderer, 0, 0, 0, 0);
        }
    };

    class Collision
    {
      public:
        static bool AABB(const SDL_Rect a, SDL_Rect b) { return SDL_HasIntersection(&a, &b) == SDL_TRUE; }
        static bool AABB(const Collider& colA, const Collider& colB)
        {
            return AABB(colA.collider, colB.collider);
        }
    };
}; // namespace component
}; // namespace engine
