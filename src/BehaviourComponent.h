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
                case Action::GoNorth: entity->getComponent<CharacterController>().goNorth(); break;
                case Action::GoSouth: entity->getComponent<CharacterController>().goSouth(); break;
                case Action::GoEast: entity->getComponent<CharacterController>().goEast(); break;
                case Action::GoWest: entity->getComponent<CharacterController>().goWest(); break;
                case Action::RotNorth:
                    entity->getComponent<CharacterController>().setDirection(Direction::North);
                    break;
                case Action::RotSouth:
                    entity->getComponent<CharacterController>().setDirection(Direction::South);
                    break;
                case Action::RotEast:
                    entity->getComponent<CharacterController>().setDirection(Direction::East);
                    break;
                case Action::RotWest:
                    entity->getComponent<CharacterController>().setDirection(Direction::West);
                    break;
                case Action::Idle: break;
            }
            lastMovementTick = ticks;
        }
    }

  private:
    int actionIdx;
    Behaviour behaviour;
    Uint64 lastMovementTick;
};
