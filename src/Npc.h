#pragma once

#include "Game.h"
#include "absl/log/log.h"
#include "src/Asset.h"
#include "src/CharacterController.h"
#include "src/ColliderComponent.h"
#include "src/ECS.h"
#include "src/RoutineComponent.h"
#include "src/SpriteComponent.h"
#include "src/TransformComponent.h"
#include "src/Vector2.h"

#include <memory>
#include <vector>

extern Manager manager;

inline void AddNpc(Point2 point2, AssetID textureID, std::vector<std::shared_ptr<Action>> actions)
{
    auto loadTextureRes = Game::assetManager->getOrLoad<Texture>(textureID);
    LOG_IF(ERROR, !loadTextureRes.ok()) << loadTextureRes.status().message();

    auto npc = manager.addEntity();
    npc->addGroup(Game::groupPlayer);
    npc->addGroup(Game::groupCollider);

    auto transform = npc->addComponent<TransformComponent>(Point2(440, 220), Size2(44, 44));
    auto sprite = &npc->addComponent<SpriteComponent>("npc", Size2(16, 20), Point2(2, 325));
    npc->addComponent<CharacterController>();
    npc->addComponent<ColliderComponent>("Npc");

    // std::vector<std::unique_ptr<Action>> actions;
    // actions.push_back(std::make_unique<GoSouthAction>());
    // 
    // actions.push_back(std::make_unique<SetDirectionAction>(Direction::East));
    // actions.push_back(std::make_unique<GoEastAction>());
    // actions.push_back(std::make_unique<GoEastAction>());

    // actions.push_back(std::make_unique<SetDirectionAction>(Direction::West));
    // actions.push_back(std::make_unique<GoWestAction>());
    // actions.push_back(std::make_unique<GoWestAction>());
    // 
    // actions.push_back(std::make_unique<SetDirectionAction>(Direction::North));
    // actions.push_back(std::make_unique<GoNorthAction>());

    // npc->addComponent<RoutineComponent>(std::move(actions));

    sprite->addAnimation(
        "walk_up",
        new Animation(200, std::vector<Point2> { Point2(22, 325), Point2(91, 325), Point2(108, 325) }));
    sprite->addAnimation(
        "walk_down",
        new Animation(200, std::vector<Point2> { Point2(2, 325), Point2(57, 325), Point2(74, 325) }));
    sprite->addAnimation(
        "walk_lateral",
        new Animation(200, std::vector<Point2> { Point2(40, 325), Point2(126, 325) }));
};
