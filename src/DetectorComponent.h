#pragma once

#include "CharacterController.h"
#include "ECS.h"
#include "Game.h"
#include "TransformComponent.h"
#include "absl/log/log.h"
#include "src/BehaviourComponent.h"

extern Manager manager;
extern Game game;

class DetectorComponent: public Component
{
  public:
    DetectorComponent(int xCellThreshold, int yCellThreshold):
    // DetectorComponent(int xCellThreshold, int yCellThreshold, Behaviour behaviour):
        xCellThreshold(xCellThreshold),
        yCellThreshold(yCellThreshold)
        // behaviour(behaviour),
        // behaviourTriggered(false)
    {
    }

    void init() override
    {
        LOG_IF(ERROR, !entity->hasComponent<CharacterController>()) << "Missing character controller";
        LOG_IF(ERROR, !entity->hasComponent<TransformComponent>()) << "Missing transform component";
        character = &entity->getComponent<CharacterController>();
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        // TODO: Improve the way player is obtained.
        auto& player = manager.getGroup(Game::groupPlayer)[0];
        auto& playerPoint2 = player->getComponent<TransformComponent>().point2;

        auto distance = playerPoint2 - transform->point2;
        auto yCellDiff = distance.y / 44;
        auto XCellDiff = distance.x / 44;

        if (distance.x == 0 && transform->direction == Direction::South
            && transform->point2.y < playerPoint2.y && yCellDiff <= yCellThreshold)
        {
            LOG(INFO) << "Detected South";

            Game::lockCharacterControllers();
            // execBehaviour = true;
        }
        else if (distance.x == 0 && transform->direction == Direction::North
            && transform->point2.y > playerPoint2.y && -yCellDiff <= yCellThreshold)
        {
            LOG(INFO) << "Detected North";
            // execBehaviour = true;
        }
        else if (distance.y == 0 && transform->direction == Direction::West && transform->point2.x > playerPoint2.x
            && -XCellDiff <= xCellThreshold)
        {
            LOG(INFO) << "Detected West";
            // execBehaviour = true;
        }
        else if (distance.y == 0 && transform->direction == Direction::East && transform->point2.x < playerPoint2.x
            && XCellDiff <= xCellThreshold)
        {
            LOG(INFO) << "Detected East";
            // execBehaviour = true;
        } else 
        {
            Game::unlockCharacterControllers();
        }
    }

  private:
    CharacterController* character;
    TransformComponent* transform;
    int xCellThreshold;
    int yCellThreshold;
    // Behaviour behaviour;
    // bool behaviourTriggered;
    // bool execBehaviour;
    
    
    void approachPlayer()
    {

    }
};
