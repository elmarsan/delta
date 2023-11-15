#pragma once

#include "CharacterController.h"
#include "ECS.h"
#include "absl/log/log.h"
#include "src/TransformComponent.h"

enum class Action
{
    GoNorth,
    GoSouth,
    GoEast,
    GoWest,
    RotNorth,
    RotSouth,
    RotEast,
    RotWest,
    Idle,
};

using Behaviour = std::vector<Action>;

class BehaviourComponent: public Component
{
  public:
    BehaviourComponent(Behaviour behaviour): behaviour(std::move(behaviour)), actionIdx(0) {}

    void init() override 
    {
        LOG_IF(ERROR, !entity->hasComponent<TransformComponent>()) << "Missing transform component";
        character = &entity->getComponent<CharacterController>();
    }

    void update() override
    {
        if (behaviour.size() == 0)
            return;
        // TODO: Timer
        int delay = 1000;
        Uint64 ticks = SDL_GetTicks64();
        if (ticks - lastMovementTick > delay)
        {
            auto action = behaviour[actionIdx++];
            if (actionIdx >= behaviour.size())
                actionIdx = 0;

            switch (action)
            {
                case Action::GoNorth: character->go(Direction::North); break;
                case Action::GoSouth: character->go(Direction::South); break;
                case Action::GoEast: character->go(Direction::East); break;
                case Action::GoWest: character->go(Direction::West); break;
                case Action::RotNorth: character->setDirection(Direction::North); break;
                case Action::RotSouth: character->setDirection(Direction::South); break;
                case Action::RotEast: character->setDirection(Direction::East); break;
                case Action::RotWest: character->setDirection(Direction::West); break;
                case Action::Idle: break;
            }
            lastMovementTick = ticks;
        }
    }

  private:
    CharacterController* character;
    int actionIdx;
    Behaviour behaviour;
    Uint64 lastMovementTick;
};
