#pragma once

#include "ECS.h"
#include "Vector2D.h"

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT
};

struct TransformComponent: Component
{
    Vector2D position;
    Vector2D velocity;
    Direction direction;
    
    int speed = 2;

    void init() override
    {
        direction = Direction::DOWN;
        position.x = 0;
        position.y = 0;
    }

    void update() override
    {
        position.y += velocity.y * speed;
        position.x += velocity.x * speed;
    }
};
