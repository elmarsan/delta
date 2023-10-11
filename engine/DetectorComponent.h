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
        LOG(INFO) << distance << " *** " << size2;

        // auto farX = (playerPoint2.x - transform->point2.x) / 44;
        // auto farY = (playerPoint2.y - transform->point2.y)  / 44;
        
        // LOG(INFO) << "FarY" << farY;
        // LOG(INFO) << "FarY" << farY;
        // if (farY <= gridY)
        // {
        // }
    }
};
