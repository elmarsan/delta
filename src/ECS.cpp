#include "ECS.h"

void Entity::addGroup(Group group)
{
    groupBitSet[group] = true;
    manager.addGroup(this, group);
}
