#include "src/CharacterController.h"
#include "src/ColliderComponent.h"
#include "src/ECS.h"
#include "src/SpriteComponent.h"
#include "src/TransformComponent.h"

class Player: public Entity
{
  public:
    Player(Manager& manager): Entity(manager) {}

    void init() override;

    inline void goNorth() const { controller->goNorth(); };
    inline void goSouth() const { controller->goSouth(); };
    inline void goEast() const { controller->goEast(); };
    inline void goWest() const { controller->goWest(); };
    inline Point2 currentPos() const { return transform->point2; }

  private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    CharacterController* controller;
    ColliderComponent* collider;

    void initAnimations();
};
