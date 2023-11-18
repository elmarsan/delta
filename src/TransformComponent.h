#pragma once

#include "ECS.h"
#include "Game.h"
#include "math/Vec2.h"

enum class Direction
{
    North,
    South,
    East,
    West,
};

struct TransformComponent: Component
{
    Point2 point2;
    Point2 vel2;
    Direction direction;

    Size2 size2;
    double speed = 2.0f;

    TransformComponent() { point2.zero(); }

    TransformComponent(Point2 point2): point2(point2), size2(Size2(44, 44)) {}

    void init() override { direction = Direction::South; }

    void update() override
    {
        point2.x += static_cast<int>(vel2.x * speed);
        point2.y += static_cast<int>(vel2.y * speed);
    }
};
