#pragma once

#include "CharacterController.h"
#include "ECS.h"
#include "Game.h"
#include "TransformComponent.h"
#include "absl/log/log.h"

extern Manager manager;

class DetectorComponent: public Component
{
  public:
    CharacterController* character;
    TransformComponent* transform;
    Size2 size2;

    DetectorComponent(Size2 size2): size2(size2) {}

    void init() override
    {
        character = &entity->getComponent<CharacterController>();
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        auto& player = manager.getGroup(Game::groupPlayer)[0];
        auto& playerPoint2 = player->getComponent<TransformComponent>().point2;

        auto distance = playerPoint2 - transform->point2;
        // distance *= -1;
        // LOG(INFO) << distance << " *** " << size2;

        switch (transform->direction)
        {
            case Direction::North:
                if (-distance.y == -size2.y)
                    // LOG(INFO) << "North detection";
                break;
            case Direction::South:
                if (distance.y <= size2.y)
                    // LOG(INFO) << "South detection";
                break;

            case Direction::East: break;
            case Direction::West: break;
        }
    }
};
