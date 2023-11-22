// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

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

    void go(Direction direction, int cells = 1);
    void setDirection(Direction direction);
    void lockMovement();
    void unlockMovement();
    bool isMoving();

  private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    Point2 targetPoint2;
    bool colliding;
    Uint64 collisionTick;
    bool locked;

    void goNorth(int cells);
    void goSouth(int cells);
    void goEast(int cells);
    void goWest(int cells);
    void collidePoint2();
    void onTargetPoint2Reached();
    bool hasCollider(Point2 point2);
};
