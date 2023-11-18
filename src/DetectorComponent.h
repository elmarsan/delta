#pragma once

#include "BehaviourComponent.h"
#include "CharacterController.h"
#include "ECS.h"
#include "Game.h"
#include "TransformComponent.h"
#include "absl/log/log.h"

extern Manager manager;

class DetectorComponent: public Component
{
  public:
    DetectorComponent(int xCellThreshold, int yCellThreshold):
        xCellThreshold(xCellThreshold), yCellThreshold(yCellThreshold), playerDetected(false)
    {
    }

    void update() override
    {
        auto character = &entity->getComponent<CharacterController>();
        if (character->isMoving())
            return;

        // TODO: Improve the way player is obtained.
        auto& player = manager.getGroup(Game::groupPlayer)[0];
        auto& playerPoint2 = player->getComponent<TransformComponent>().point2;

        auto transform = &entity->getComponent<TransformComponent>();
        auto distance = playerPoint2 - transform->point2;
        auto yCellDiff = distance.y / 44;
        auto xCellDiff = distance.x / 44;

        // Player detected in south direction.
        if (distance.x == 0 && transform->direction == Direction::South
            && transform->point2.y < playerPoint2.y && yCellDiff <= yCellThreshold)
        {
            bool reachPlayer = yCellDiff > 1 && !playerDetected;
            bool playerIsInFront = yCellDiff == 1;
            if (reachPlayer)
            {
                character->go(Direction::South, yCellDiff - 1);
                onApproachingPlayer();
            }
            else if (playerIsInFront && !playerDetected)
            {
                onPlayerInFront();
            }
        }
        // Player detected in north direction.
        else if (distance.x == 0 && transform->direction == Direction::North
                 && transform->point2.y > playerPoint2.y && -yCellDiff <= yCellThreshold)
        {
            bool reachPlayer = -yCellDiff > 1 && !playerDetected;
            bool playerIsInFront = -yCellDiff == 1;
            if (reachPlayer)
            {
                character->go(Direction::North, -yCellDiff - 1);
                onApproachingPlayer();
            }
            else if (playerIsInFront && !playerDetected)
            {
                onPlayerInFront();
            }
        }
        // Player detected in west direction.
        else if (distance.y == 0 && transform->direction == Direction::West
                 && transform->point2.x > playerPoint2.x && -xCellDiff <= xCellThreshold)
        {
            bool reachPlayer = -xCellDiff > 1 && !playerDetected;
            bool playerIsInFront = -xCellDiff == 1;
            if (reachPlayer)
            {
                character->go(Direction::West, -xCellDiff - 1);
                onApproachingPlayer();
            }
            else if (playerIsInFront && !playerDetected)
            {
                onPlayerInFront();
            }
        }
        // Player detected in south direction.
        else if (distance.y == 0 && transform->direction == Direction::East
                 && transform->point2.x < playerPoint2.x && xCellDiff <= xCellThreshold)
        {
            bool reachPlayer = xCellDiff > 1 && !playerDetected;
            bool playerIsInFront = xCellDiff == 1;
            if (reachPlayer)
            {
                character->go(Direction::East, xCellDiff - 1);
                onApproachingPlayer();
            }
            else if (playerIsInFront && !playerDetected)
            {
                onPlayerInFront();
            }
        }
    }

  private:
    int xCellThreshold;
    int yCellThreshold;
    bool playerDetected;

    void onPlayerInFront()
    {
        playerDetected = true;
        for (auto& actor: manager.getGroup(Game::groupPlayer))
        {
            if (actor != entity)
                actor->getComponent<CharacterController>().unlockMovement();
        }
    }

    void onApproachingPlayer()
    {
        if (entity->hasComponent<BehaviourComponent>())
            entity->removeComponent<BehaviourComponent>();

        for (auto& actor: manager.getGroup(Game::groupPlayer))
        {
            if (actor != entity)
                actor->getComponent<CharacterController>().lockMovement();
        }
    }
};
