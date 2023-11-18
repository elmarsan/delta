#pragma once

#include "CharacterController.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "absl/log/log.h"
#include "math/Plane2.h"
#include "math/Vec2.h"
#include <memory>
#include <utility>

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
    Plane2<int>* plane2;
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

            auto character = &entity->getComponent<CharacterController>();
            auto transform = &entity->getComponent<TransformComponent>();
            switch (action)
            {
                case Action::GoNorth: 
                    if (plane2 != nullptr)
                    {
                        auto target = Point2(transform->point2.x, transform->point2.y - 44);
                        if (plane2->contains(target))
                            character->go(Direction::North); 
                        else 
                            LOG(INFO) << "Plane2 limit";
                    }
                    break;
                case Action::GoSouth: 
                    if (plane2 != nullptr)
                    {
                        auto target = Point2(transform->point2.x, transform->point2.y + 44);
                        if (plane2->contains(target))
                            character->go(Direction::South); 
                        else 
                            LOG(INFO) << "Plane2 limit";
                    }
                    break;
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
    int actionIdx;
    Behaviour behaviour;
    Uint64 lastMovementTick;
};
