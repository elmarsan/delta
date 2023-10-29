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
    CharacterController() = default;
    
    void init() override;
    void update() override;

    bool hasCollider(Point2 point2);
    void goNorth();
    void goSouth();
    void goEast();
    void goWest();
    void setDirection(Direction direction);
    void collidePoint2();

  private:
    CharacterController(const CharacterController&) = delete;
    void operator=(const CharacterController&) = delete;

    TransformComponent* transform;
    SpriteComponent* sprite;
    Point2 targetPoint2;
    bool colliding;
    Uint64 collisionTick;

    void onTargetPoint2Reached();
};
