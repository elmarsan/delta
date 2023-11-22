// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "CharacterController.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "absl/log/log.h"
#include "math/Plane2.h"
#include "math/Vec2.h"

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
    Random,
};

using Behaviour = std::vector<Action>;

class BehaviourComponent: public Component
{
  public:
    Plane2* plane2;
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

            if (action == Action::Random)
            {
                std::srand(std::time(nullptr));
                int rand_action = std::rand() % 8;
                action = static_cast<Action>(rand_action);
            }
            switch (action)
            {
                case Action::GoNorth:
                    if (plane2 != nullptr)
                    {
                        auto target = Point2(transform->point2.x, transform->point2.y - 44);
                        if (plane2->contains(target))
                            character->go(Direction::North);
                        else
                            LOG(INFO) << "Plane2 north limit";
                    }
                    break;
                case Action::GoSouth:
                    if (plane2 != nullptr)
                    {
                        auto target = Point2(transform->point2.x, transform->point2.y + 44);
                        if (plane2->contains(target))
                            character->go(Direction::South);
                        else
                            LOG(INFO) << "Plane2 south limit";
                    }
                    break;
                case Action::GoEast:
                    if (plane2 != nullptr)
                    {
                        auto target = Point2(transform->point2.x + 44, transform->point2.y);
                        if (plane2->contains(target))
                            character->go(Direction::East);
                        else
                            LOG(INFO) << "Plane2 east limit";
                    }
                    break;
                case Action::GoWest:
                    if (plane2 != nullptr)
                    {
                        auto target = Point2(transform->point2.x - 44, transform->point2.y);
                        if (plane2->contains(target))
                            character->go(Direction::West);
                        else
                            LOG(INFO) << "Plane2 west limit";
                    }
                    break;
                case Action::RotNorth:
                    if (transform->direction != Direction::North)
                        character->setDirection(Direction::North);
                    break;
                case Action::RotSouth:
                    if (transform->direction != Direction::South)
                        character->setDirection(Direction::South);
                    break;
                case Action::RotEast:
                    if (transform->direction != Direction::East)
                        character->setDirection(Direction::East);
                    break;
                case Action::RotWest:
                    if (transform->direction != Direction::West)
                        character->setDirection(Direction::West);
                    break;
                case Action::Idle: break;
                case Action::Random: break;
            }
            lastMovementTick = ticks;
        }
    }

  private:
    int actionIdx;
    Behaviour behaviour;
    Uint64 lastMovementTick;
};
