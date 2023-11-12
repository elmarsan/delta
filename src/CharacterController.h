#pragma once

#include "ECS.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

extern Manager manager;

class CharacterController: public Component
{
  public:
    void init() override;
    void update() override;

    void goNorth();
    void goSouth();
    void goEast();
    void goWest();
    void setDirection(Direction direction);
    void lockMovement();
    void unlockMovement();

  private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    Point2 targetPoint2;
    bool colliding;
    Uint64 collisionTick;
    bool locked = false;

    void collidePoint2();
    void onTargetPoint2Reached();
    bool hasCollider(Point2 point2);
};
