#pragma once

#include "CharacterController.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "absl/log/log.h"

#include <SDL2/SDL_timer.h>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

/*
 * 1. Go to player position
 * 2. Dialog
 *
 */

class BehaviourComponent: public Component
{
  public:
    CharacterController* character;
    TransformComponent* transform;
    Uint64 lastMovementTick;

    void init() override
    {
        srand(time(NULL));
        character = &entity->getComponent<CharacterController>();
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        int delay = 500;
        Uint64 ticks = SDL_GetTicks64();
        if (ticks - lastMovementTick > delay)
        {
            int randAction = rand() % 4 + 1;
            LOG(INFO) << "Player random action: " << randAction;
            switch (randAction)
            {
                case 0: character->goNorth(); break;
                case 1: character->goSouth(); break;
                case 2: character->goEast(); break;
                case 3: character->goWest(); break;
            }
            lastMovementTick = ticks;
        }
    }
};
