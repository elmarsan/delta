// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

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
