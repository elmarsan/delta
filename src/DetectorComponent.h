#pragma once

#include "CharacterController.h"
#include "ECS.h"
#include "Game.h"
#include "TransformComponent.h"
#include "absl/log/log.h"
#include "src/BehaviourComponent.h"

extern Manager manager;

class DetectorComponent: public Component
{
  public:
    DetectorComponent(int xCellThreshold, int yCellThreshold):
        xCellThreshold(xCellThreshold), yCellThreshold(yCellThreshold), playerDetected(false)
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
        if (character->isMoving())
            return;

        // TODO: Improve the way player is obtained.
        auto& player = manager.getGroup(Game::groupPlayer)[0];
        auto& playerPoint2 = player->getComponent<TransformComponent>().point2;

        auto distance = playerPoint2 - transform->point2;
        auto yCellDiff = distance.y / 44;
        auto xCellDiff = distance.x / 44;

        if (distance.x == 0 && transform->direction == Direction::South
            && transform->point2.y < playerPoint2.y && yCellDiff <= yCellThreshold)
        {
            if (yCellDiff > 1 && !playerDetected)
            {
                LOG(INFO) << "Detected South: " << yCellDiff;
                character->go(Direction::South, yCellDiff - 1);
                for (auto& actor: manager.getGroup(Game::groupPlayer))
                {
                    if (actor != entity)
                        actor->getComponent<CharacterController>().lockMovement();
                }
            }
            else if (yCellDiff == 1 && !playerDetected)
            {
                playerDetected = true;
                LOG(INFO) << "Player reached";
                for (auto& actor: manager.getGroup(Game::groupPlayer))
                {
                    actor->getComponent<CharacterController>().unlockMovement();
                }
            }
        }
        else if (distance.x == 0 && transform->direction == Direction::North
                 && transform->point2.y > playerPoint2.y && -yCellDiff <= yCellThreshold)
        {
            if (-yCellDiff > 1 && !playerDetected)
            {
                LOG(INFO) << "Detected North" << -yCellDiff;
                character->go(Direction::North, -yCellDiff - 1);
                for (auto& actor: manager.getGroup(Game::groupPlayer))
                {
                    if (actor != entity)
                        actor->getComponent<CharacterController>().lockMovement();
                }
            }
            else if (-yCellDiff == 1 && !playerDetected)
            {
                playerDetected = true;
                LOG(INFO) << "Player reached";
                for (auto& actor: manager.getGroup(Game::groupPlayer))
                {
                    actor->getComponent<CharacterController>().unlockMovement();
                }
                entity->removeComponent<BehaviourComponent>();
            }
        }
        else if (distance.y == 0 && transform->direction == Direction::West
                 && transform->point2.x > playerPoint2.x && -xCellDiff <= xCellThreshold)
        {
            LOG(INFO) << "Detected West";
        }
        else if (distance.y == 0 && transform->direction == Direction::East
                 && transform->point2.x < playerPoint2.x && xCellDiff <= xCellThreshold)
        {
            if (xCellDiff > 1 && !playerDetected)
            {
                LOG(INFO) << "Detected East" << xCellDiff;
                character->go(Direction::East, xCellDiff - 1);
                for (auto& actor: manager.getGroup(Game::groupPlayer))
                {
                    if (actor != entity)
                        actor->getComponent<CharacterController>().lockMovement();
                }
            }
            else if (xCellDiff == 1 && !playerDetected)
            {
                playerDetected = true;
                LOG(INFO) << "Player reached";
                for (auto& actor: manager.getGroup(Game::groupPlayer))
                {
                    actor->getComponent<CharacterController>().unlockMovement();
                }
                entity->removeComponent<BehaviourComponent>();
            }
        }
    }

  private:
    CharacterController* character;
    TransformComponent* transform;
    int xCellThreshold;
    int yCellThreshold;
    bool playerDetected;
};
