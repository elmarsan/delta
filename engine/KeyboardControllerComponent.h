#pragma once

#include "delta/Game.h"
#include "ECS.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include <SDL2/SDL_events.h>

class KeyboardControllerComponent: public Component
{
  public:
    TransformComponent* transform;
    SpriteComponent* sprite;
    int ticks;
    int keyEventDelay = 50;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        // Key event cooldown
        // if (Game::event.type == SDL_KEYDOWN || Game::event.type == SDLK_UP)
        // {
        //     if (static_cast<int>(SDL_GetTicks64()) > ticks + keyEventDelay)
        //         ticks = SDL_GetTicks64();
        //     else
        //         return;
        // }

        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
                case SDLK_UP: {
                    if (transform->direction != Direction::UP)
                    {
                        transform->direction = Direction::UP;
                        transform->vel2.zero();
                        transform->speed = 0;
                    }
                    else
                    {
                        sprite->setAnimation("walk_up");
                        transform->speed = 2;
                        transform->direction = Direction::UP;
                        transform->vel2.y = -1;
                        transform->vel2.x = 0;
                    }
                    break;
                }
                case SDLK_DOWN: {
                    if (transform->direction != Direction::DOWN)
                    {
                        transform->direction = Direction::DOWN;
                        transform->vel2.zero();
                        transform->speed = 0;
                    }
                    else
                    {
                        sprite->setAnimation("walk_down");
                        transform->speed = 2;
                        transform->direction = Direction::DOWN;
                        transform->vel2.y = 1;
                        transform->vel2.x = 0;
                    }
                    break;
                }
                case SDLK_LEFT: {
                    if (transform->direction != Direction::LEFT)
                    {
                        transform->direction = Direction::LEFT;
                        transform->vel2.zero();
                        transform->speed = 0;
                    }
                    else
                    {
                        sprite->setAnimation("walk_lateral");
                        transform->speed = 2;
                        transform->direction = Direction::LEFT;
                        transform->vel2.x = -1;
                        transform->vel2.y = 0;
                    }
                    break;
                }
                case SDLK_RIGHT: {
                    if (transform->direction != Direction::RIGHT)
                    {
                        transform->direction = Direction::RIGHT;
                        transform->vel2.zero();
                        transform->speed = 0;
                    }
                    else
                    {
                        sprite->setAnimation("walk_lateral", SDL_FLIP_HORIZONTAL);
                        transform->speed = 2;
                        transform->direction = Direction::RIGHT;
                        transform->vel2.x = 1;
                        transform->vel2.y = 0;
                    }
                    break;
                }
            }
        }

        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
                case SDLK_UP: transform->vel2.zero(); break;
                case SDLK_DOWN: transform->vel2.zero(); break;
                case SDLK_LEFT: transform->vel2.zero(); break;
                case SDLK_RIGHT: transform->vel2.zero(); break;
            }
            sprite->stopAnimation();
        }
    }
};
