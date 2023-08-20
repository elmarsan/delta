#pragma once

#include "ECS.h"
#include "Vector2D.h"

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

struct TransformComponent: Component
{
    Vector2D position;
    Vector2D velocity;
    Direction direction;

    int width;
    int height;
    int scale;
    int speed = 2;

    TransformComponent() { position.zero(); }

    TransformComponent(Vector2D position, int w, int h, int scale):
        position(position), width(w), height(h), scale(scale)
    {
    }

    void init() override { direction = Direction::DOWN; }

    void update() override
    {
        position.y += velocity.y * speed;
        position.x += velocity.x * speed;
    }
};
