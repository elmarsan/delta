// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "Npc.h"

#include "src/ColliderComponent.h"
#include "src/DetectorComponent.h"
#include "src/ECS.h"

#include <memory>

// void (addNpcPoint2 point2, NpcType npcType, Behaviour behaviour, bool detector)
std::shared_ptr<Entity> addNpc(Point2 point2, NpcType npcType, Behaviour behaviour, bool detector)
{
    auto npc = manager.addEntity();
    npc->addComponent<TransformComponent>(point2);
    // TODO: Do not hardcode npc texture src.
    auto sprite = &npc->addComponent<SpriteComponent>("npc", Size2(16, 20), Point2(2, 325));
    npc->addComponent<CharacterController>();
    npc->addComponent<ColliderComponent>("Npc");
    npc->addComponent<BehaviourComponent>(std::move(behaviour));

    auto animations = getNpcAnimations(npcType);
    LOG_IF(ERROR, !animations.ok()) << animations.status().message();

    if (animations.ok())
    {
        for (auto& a: animations.value())
            sprite->addAnimation(a.first, a.second);
    }

    if (detector)
        npc->addComponent<DetectorComponent>(5, 4);

    npc->addGroup(Game::groupPlayer);
    npc->addGroup(Game::groupCollider);
    return npc;
}

absl::StatusOr<std::map<std::string, Animation*>> getNpcAnimations(NpcType npcType)
{
    switch (npcType)
    {
        case NpcType::FatManBlue:
            return std::map<std::string, Animation*> {
                { "walk_up",
                  new Animation(200,
                                std::vector<Point2> { Point2(22, 325), Point2(91, 325), Point2(108, 325) }) },
                { "walk_down",
                  new Animation(200,
                                std::vector<Point2> { Point2(2, 325), Point2(57, 325), Point2(74, 325) }) },
                { "walk_lateral",
                  new Animation(200, std::vector<Point2> { Point2(40, 325), Point2(126, 325) }) }
            };
            break;
        default: return absl::NotFoundError("No animations found");
    }
}
