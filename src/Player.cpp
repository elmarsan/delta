// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "Player.h"
#include "Engine.h"
#include "src/Game.h"

void Player::init()
{
    transform = &addComponent<TransformComponent>(Point2(528, 352));
    sprite = &addComponent<SpriteComponent>("p1", Size2(14, 21));
    controller = &addComponent<CharacterController>();
    collider = &addComponent<ColliderComponent>("Player");
    addGroup(Game::groupPlayer);
    addGroup(Game::groupCollider);

    initAnimations();
}

void Player::initAnimations()
{
    sprite->addAnimation(
        "walk_up", new Animation(150, std::vector<Point2> { Point2(15, 0), Point2(15, 22), Point2(15, 44) }));
    sprite->addAnimation(
        "walk_down", new Animation(150, std::vector<Point2> { Point2(0, 0), Point2(0, 22), Point2(0, 44) }));
    sprite->addAnimation(
        "walk_lateral",
        new Animation(150, std::vector<Point2> { Point2(30, 0), Point2(31, 22), Point2(30, 44) }));
}
