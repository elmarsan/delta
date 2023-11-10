#pragma once

#include "ECS.h"
#include "absl/log/log.h"
#include "src/CharacterController.h"
#include "src/TransformComponent.h"
#include <memory>

struct Action
{
    virtual void execute(Entity* entity) {};
    virtual ~Action() {};
};

struct GoNorthAction: Action
{
    void execute(Entity* entity) override
    {
        if (!entity->hasComponent<CharacterController>())
        {
            LOG(ERROR) << "GoNorthAction cannot be execute without CharacterController";
            return;
        }
        LOG(INFO) << "GoNorth action";
        entity->getComponent<CharacterController>().goNorth();
    }
};

struct GoSouthAction: Action
{
    void execute(Entity* entity) override
    {
        if (!entity->hasComponent<CharacterController>())
        {
            LOG(ERROR) << "GoSouthAction cannot be execute without CharacterController";
            return;
        }
        LOG(INFO) << "GoSouth action";
        entity->getComponent<CharacterController>().goSouth();
    }
};

struct GoEastAction: Action
{
    void execute(Entity* entity) override
    {
        if (!entity->hasComponent<CharacterController>())
        {
            LOG(ERROR) << "GoEastAction cannot be execute without CharacterController";
            return;
        }
        LOG(INFO) << "GoEastAction action";
        entity->getComponent<CharacterController>().goEast();
    }
};

struct GoWestAction: Action
{
    void execute(Entity* entity) override
    {
        if (!entity->hasComponent<CharacterController>())
        {
            LOG(ERROR) << "GoWestAction cannot be execute without CharacterController";
            return;
        }
        LOG(INFO) << "GoWestAction action";
        entity->getComponent<CharacterController>().goWest();
    }
};

struct SetDirectionAction: Action
{
    Direction direction;

    SetDirectionAction(Direction direction): direction(direction) {} 

    void execute(Entity *entity) override
    {
        if (!entity->hasComponent<CharacterController>())
        {
            LOG(ERROR) << "SetDirectionAction cannot be execute without CharacterController";
            return;
        }
        LOG(INFO) << "SetDirectionAction action";
        entity->getComponent<CharacterController>().setDirection(direction);
    }
};

class RoutineComponent: public Component
{
  public:
    RoutineComponent(std::vector<std::unique_ptr<Action>> actions): actions(std::move(actions)), actionIdx(0) {}

    void init() override { LOG(INFO) << "Init player routine component"; }

    void update() override
    {
        if (actions.size() == 0)
            return;

        int delay = 1000;
        Uint64 ticks = SDL_GetTicks64();
        if (ticks - lastMovementTick > delay)
        {
            LOG(INFO) << "Executing action: " << actionIdx + 1 << " of: " << actions.size();
            actions[actionIdx++]->execute(entity);
            if (actionIdx >= actions.size())
                actionIdx = 0;
            lastMovementTick = ticks;
        }
    }

  private:
    int actionIdx;
    std::vector<std::unique_ptr<Action>> actions;
    Uint64 lastMovementTick;
};
