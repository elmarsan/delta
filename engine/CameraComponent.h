#include "ECS.h"
#include "TransformComponent.h"
#include "Vector2D.h"
#include "delta/Game.h"

#include <SDL.h>

class CameraComponent: public Component
{
  private:
    SDL_Rect rect;
    Vector2D pos;
    int w, h;

  public:
    CameraComponent(int w, int h): w(w), h(h) {}

    void update() override
    {
        rect.x = (pos.x) - (w * 44 / 2);
        rect.y = (pos.y) - (h * 44 / 2);
        rect.w = w * 44;
        rect.h = h * 44;
    }

    void draw() override
    {
        SDL_SetRenderDrawColor(Game::renderer, 0xff, 165, 0, 0);
        SDL_RenderDrawRect(Game::renderer, &rect);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
    }

    void setPos(Vector2D newPos)
    {
        pos.x = newPos.x;
        pos.y = newPos.y;
    }
};
