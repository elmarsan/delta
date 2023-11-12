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


// #include "CharacterController.h"
// #include "ECS.h"
// #include "absl/log/log.h"
// #include "src/TransformComponent.h"

// extern Manager manager;

// enum class Action
// {
//     GoNorth,
//     GoSouth,
//     GoEast,
//     GoWest,
//     RotNorth,
//     RotSouth,
//     RotEast,
//     RotWest,
//     Idle
// };

// enum class PlayerDetector
// {
//     None,
//     Battle
// };

// using Routine = std::vector<Action>;

// struct Behaviour
// {
//     Routine routine;
//     PlayerDetector playerDetector;

//     bool hasRoutine() { return routine.size() > 0; }
//     bool hasPlayerDetector() { return playerDetector != PlayerDetector::None; }

//     Action getNextAction()
//     {
//         Action action = *itAction;
//         ++itAction;
//         if (itAction == routine.end())
//             itAction = routine.begin();
//         return action;
//     }

//   private:
//     std::vector<Action>::iterator itAction;
// };

// class BehaviourComponent: public Component
// {
//   private:
//     TransformComponent* transform;
//     CharacterController* character;
//     Behaviour behaviour;
//     Uint64 lastMovementTick;

//   public:
//     BehaviourComponent(Behaviour behaviour): behaviour(behaviour) {}

//     void init() override
//     {
//         LOG_IF(ERROR, !entity->hasComponent<CharacterController>()) << "Missing character controller";
//         LOG_IF(ERROR, !entity->hasComponent<TransformComponent>()) << "Missing transform component";
//         character = &entity->getComponent<CharacterController>();
//         transform = &entity->getComponent<TransformComponent>();
//     }

//     void update() override
//     {
//         if (behaviour.hasPlayerDetector())
//         {
//             // TODO: Improve the way player is obtained.
//             auto& player = manager.getGroup(Game::groupPlayer)[0];
//             auto& playerPoint2 = player->getComponent<TransformComponent>().point2;

//             auto distance = playerPoint2 - transform->point2;
//             auto yCellDiff = distance.y / 44;
//             auto XCellDiff = distance.x / 44;

//             if (distance.x == 0 && transform->direction == Direction::South
//                 && transform->point2.y < playerPoint2.y && yCellDiff <= yCellThreshold)
//             {
//                 LOG(INFO) << "Detected South";
//                 // execBehaviour = true;
//             }
//             if (distance.x == 0 && transform->direction == Direction::North
//                 && transform->point2.y > playerPoint2.y && -yCellDiff <= yCellThreshold)
//             {
//                 LOG(INFO) << "Detected North";
//                 // execBehaviour = true;
//             }
//             if (distance.y == 0 && transform->direction == Direction::West
//                 && transform->point2.x > playerPoint2.x && -XCellDiff <= xCellThreshold)
//             {
//                 LOG(INFO) << "Detected West";
//                 // execBehaviour = true;
//             }
//             if (distance.y == 0 && transform->direction == Direction::East
//                 && transform->point2.x < playerPoint2.x && XCellDiff <= xCellThreshold)
//             {
//                 LOG(INFO) << "Detected East";
//                 // execBehaviour = true;
//             }
//         }

//         // TODO: Attach detector.
//         if (behaviour.hasRoutine())
//             return;

//         // if (behaviour.size() == 0)
//         //     return;
//         // // TODO: Timer
//         // int delay = 1000;
//         // Uint64 ticks = SDL_GetTicks64();
//         // if (ticks - lastMovementTick > delay)
//         // {
//         //     auto action = behaviour[actionIdx++];
//         //     if (actionIdx >= behaviour.size())
//         //         actionIdx = 0;
//         //     switch (action)
//         //     {
//         //         case Action::GoNorth: entity->getComponent<CharacterController>().goNorth(); break;
//         //         case Action::GoSouth: entity->getComponent<CharacterController>().goSouth(); break;
//         //         case Action::GoEast: entity->getComponent<CharacterController>().goEast(); break;
//         //         case Action::GoWest: entity->getComponent<CharacterController>().goWest(); break;
//         //         case Action::RotNorth:
//         //             entity->getComponent<CharacterController>().setDirection(Direction::North);
//         //             break;
//         //         case Action::RotSouth:
//         //             entity->getComponent<CharacterController>().setDirection(Direction::South);
//         //             break;
//         //         case Action::RotEast:
//         //             entity->getComponent<CharacterController>().setDirection(Direction::East);
//         //             break;
//         //         case Action::RotWest:
//         //             entity->getComponent<CharacterController>().setDirection(Direction::West);
//         //             break;
//         //         case Action::Idle: break;
//         //     }
//         //     lastMovementTick = ticks;
//         // }
//     }
// }
