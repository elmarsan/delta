#include "CharacterController.h"

#include "absl/log/log.h"
#include "src/ColliderComponent.h"
#include "src/TransformComponent.h"

void CharacterController::init()
{
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
    locked = false;
}

void CharacterController::update()
{
    if (transform->point2 == targetPoint2)
        onTargetPoint2Reached();

    // TODO: Timer
    const int FPS = 60;
    const int frameDelay = 10000 / FPS;
    int frameTime = SDL_GetTicks64() - collisionTick;
    if (colliding && frameDelay <= frameTime)
    {
        sprite->stopAnimation();
        colliding = false;
    }
}

// TODO: Improve collision system and move this function to somewhere.
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

void CharacterController::go(Direction direction, int cells)
{
    if (locked)
        return;

    if (!targetPoint2.isZero())
        return;

    if (transform->direction != direction)
    {
        setDirection(direction);
        return;
    }

    switch (direction)
    {
        case Direction::North: goNorth(cells); break;
        case Direction::South: goSouth(cells); break;
        case Direction::East: goEast(cells); break;
        case Direction::West: goWest(cells); break;
    }
}

void CharacterController::goNorth(int cells)
{
    auto newTarget = Point2(transform->point2.x, transform->point2.y - 44 * cells);
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

void CharacterController::goSouth(int cells)
{
    auto newTarget = Point2(transform->point2.x, transform->point2.y + 44 * cells);
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

void CharacterController::goEast(int cells)
{
    auto newTarget = Point2(transform->point2.x + 44 * cells, transform->point2.y);
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

void CharacterController::goWest(int cells)
{
    auto newTarget = Point2(transform->point2.x - 44 * cells, transform->point2.y);
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

void CharacterController::setDirection(Direction direction)
{
    if (locked)
        return;

    switch (direction)
    {
        case Direction::North:
            transform->direction = Direction::North;
            sprite->setAnimationFrame("walk_up", 0);
            break;
        case Direction::South:
            transform->direction = Direction::South;
            sprite->setAnimationFrame("walk_down", 0);
            break;
        case Direction::East:
            transform->direction = Direction::East;
            sprite->setAnimationFrame("walk_lateral", 0, SDL_FLIP_HORIZONTAL);
            break;
        case Direction::West:
            transform->direction = Direction::West;
            sprite->setAnimationFrame("walk_lateral", 0);
            break;
    }
}

void CharacterController::collidePoint2()
{
    colliding = true;
    collisionTick = SDL_GetTicks64();
}

void CharacterController::lockMovement()
{
    locked = true;
}

void CharacterController::unlockMovement()
{
    locked = false;
}

bool CharacterController::isMoving()
{
    return targetPoint2 != Point2();
}
