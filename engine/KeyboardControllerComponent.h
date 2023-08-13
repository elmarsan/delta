#pragma once

#include "../delta/Game.h"
#include "ECS.h"
#include "TransformComponent.h"

#include <SDL.h>

namespace engine
{
namespace component
{
    class KeyboardController: public Component
    {
      public:
        Transform* transform;

        void init() override { transform = &entity->getComponent<Transform>(); }

        void update() override
        {
            if (Game::event.type == SDL_KEYDOWN)
            {
                switch (Game::event.key.keysym.sym)
                {
                    case SDLK_UP: {
                        transform->direction = Direction::UP;
                        transform->velocity.y = -1;
                        transform->velocity.x = 0;
                        break;
                    }
                    case SDLK_DOWN: {
                        transform->direction = Direction::DOWN;
                        transform->velocity.y = 1;
                        transform->velocity.x = 0;
                        break;
                    }
                    case SDLK_LEFT: {
                        transform->direction = Direction::LEFT;
                        transform->velocity.x = -1;
                        transform->velocity.y = 0;
                        break;
                    }
                    case SDLK_RIGHT: {
                        transform->direction = Direction::RIGHT;
                        transform->velocity.x = 1;
                        transform->velocity.y = 0;
                        break;
                    }
                }
            }

            if (Game::event.type == SDL_KEYUP)
            {
                switch (Game::event.key.keysym.sym)
                {
                    case SDLK_UP: transform->velocity.y = 0; break;
                    case SDLK_DOWN: transform->velocity.y = 0; break;
                    case SDLK_LEFT: transform->velocity.x = 0; break;
                    case SDLK_RIGHT: transform->velocity.x = 0; break;
                }
            }
        }
    };

}; // namespace component
}; // namespace engine
