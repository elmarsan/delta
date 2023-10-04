#pragma once

#include "ECS.h"
#include "Game.h"
#include "Vector2.h"

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

struct TransformComponent: Component
{
    Point2 point2;
    Point2 vel2;
    Direction direction;

    int width;
    int height;
    int scale;
    int speed = 2;

    TransformComponent() { point2.zero(); }

    TransformComponent(Point2 point2, int w, int h, int scale):
        point2(point2), width(w), height(h), scale(scale)
    {
    }

    void init() override { direction = Direction::DOWN; }

    void update() override
    {
        point2.x += vel2.x * speed;
        point2.y += vel2.y * speed;
    }
};
