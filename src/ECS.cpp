// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "ECS.h"

void Entity::addGroup(Group group)
{
    groupBitSet[group] = true;
    manager.addGroup(this, group);
}
