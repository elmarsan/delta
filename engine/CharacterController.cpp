#include "CharacterController.h"

void CharacterController::init()
{
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
}

void CharacterController::update() 
{
    if (transform->point2 == targetPoint2)
        onTargetPoint2Reached();

    const int FPS = 60;
    const int frameDelay = 10000 / FPS;
    int frameTime = SDL_GetTicks64() - collisionTick;
    if (colliding && frameDelay <= frameTime)
    {
        sprite->stopAnimation();
        colliding = false;
    }
}

bool CharacterController::hasCollider(Point2 point2)
{
    for (auto& c: manager.getGroup(Game::groupCollider))
    {
        if (c->hasGroup(Game::groupCollider))
        {
            auto& transform = c->getComponent<TransformComponent>();
            if (transform.point2 == point2)
            {
                LOG(INFO) << "Point:" << transform.point2 << " has collider";
                return true;
            }
        }
    }

    return false;
}

void CharacterController::onTargetPoint2Reached()
{
    transform->vel2.zero();
    targetPoint2.zero();
    sprite->stopAnimation();
}

void CharacterController::goNorth()
{
    if (!targetPoint2.isZero())
        return;

    if (transform->direction != Direction::North)
    {
        transform->direction = Direction::North;
        sprite->setAnimationFrame("walk_up", 0);
        return;
    }

    auto newTarget = Point2(transform->point2.x, transform->point2.y - 44);
    sprite->setAnimation("walk_up");
    if (!hasCollider(newTarget))
    {
        transform->speed = 2;
        transform->direction = Direction::North;
        transform->vel2.y = -1;
        transform->vel2.x = 0;
        targetPoint2 = newTarget;
    }
    else
        collidePoint2();
}

void CharacterController::goSouth()
{
    if (!targetPoint2.isZero())
        return;

    if (transform->direction != Direction::South)
    {
        transform->direction = Direction::South;
        sprite->setAnimationFrame("walk_down", 0);
        return;
    }

    auto newTarget = Point2(transform->point2.x, transform->point2.y + 44);
    sprite->setAnimation("walk_down");
    if (!hasCollider(newTarget))
    {
        transform->speed = 2;
        transform->direction = Direction::South;
        transform->vel2.y = 1;
        transform->vel2.x = 0;
        targetPoint2 = newTarget;
    }
    else
        collidePoint2();
}

void CharacterController::goEast()
{
    if (!targetPoint2.isZero())
        return;

    if (transform->direction != Direction::East)
    {
        transform->direction = Direction::East;
        sprite->setAnimationFrame("walk_lateral", 0, SDL_FLIP_HORIZONTAL);
        return;
    }

    auto newTarget = Point2(transform->point2.x + 44, transform->point2.y);
    sprite->setAnimation("walk_lateral", SDL_FLIP_HORIZONTAL);
    if (!hasCollider(newTarget))
    {
        transform->speed = 2;
        transform->direction = Direction::East;
        transform->vel2.x = 1;
        transform->vel2.y = 0;
        targetPoint2 = newTarget;
    }
    else
        collidePoint2();
}

void CharacterController::goWest()
{
    if (!targetPoint2.isZero())
        return;

    if (transform->direction != Direction::West)
    {
        transform->direction = Direction::West;
        sprite->setAnimationFrame("walk_lateral", 0);
        return;
    }

    auto newTarget = Point2(transform->point2.x - 44, transform->point2.y);
    sprite->setAnimation("walk_lateral");
    if (!hasCollider(newTarget))
    {
        transform->speed = 2;
        transform->direction = Direction::West;
        transform->vel2.x = -1;
        transform->vel2.y = 0;
        targetPoint2 = newTarget;
    }
    else
        collidePoint2();
}

void CharacterController::collidePoint2()
{
    colliding = true;
    collisionTick = SDL_GetTicks64();
}
