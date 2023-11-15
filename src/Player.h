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

    inline void go(Direction direction) const { controller->go(direction); };
    inline Point2 currentPos() const { return transform->point2; }

  private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    CharacterController* controller;
    ColliderComponent* collider;

    void initAnimations();
};
