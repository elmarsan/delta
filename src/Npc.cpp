#include "Npc.h"

#include "src/ColliderComponent.h"

void addNpc(Point2 point2, NpcType npcType, Behaviour behaviour)
{
    auto loadTextureRes = Game::assetManager->getOrLoad<Texture>("npc");
    LOG_IF(ERROR, !loadTextureRes.ok()) << loadTextureRes.status().message();

    auto npc = manager.addEntity();
    auto transform = &npc->addComponent<TransformComponent>(point2);
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

    npc->addGroup(Game::groupPlayer);
    npc->addGroup(Game::groupCollider);
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
