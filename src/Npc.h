#pragma once

#include "BehaviourComponent.h"
#include "ECS.h"
#include "math/Vec2.h"

#include <memory>
#include <vector>

extern Manager manager;

enum class NpcType
{
    FatManBlue
};

// void addNpc(Point2 point2, NpcType npcType, Behaviour behaviour, bool detector = true);
std::shared_ptr<Entity> addNpc(Point2 point2, NpcType npcType, Behaviour behaviour, bool detector = true);

absl::StatusOr<std::map<std::string, Animation*>> getNpcAnimations(NpcType npcType);
