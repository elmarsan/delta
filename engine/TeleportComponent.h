#pragma once

#include "delta/Game.h"
#include "engine/ECS.h"
#include "engine/TransformComponent.h"
#include "engine/Vector2D.h"

class TeleportComponent: public Component
{
  public:
    SDL_Rect rect;
    Vector2D destiny;
    std::string mapID;
    std::string level;

    TeleportComponent(Vector2D source, Vector2D destiny, std::string mapID, std::string level = ""):
        destiny(destiny), mapID(mapID), level(level)
    {
        rect = SDL_Rect { source.x, source.y, 44, 44 };
    }

    void draw() override
    {
        SDL_SetRenderDrawColor(Game::renderer, 90, 34, 139, 0);
        SDL_RenderFillRect(Game::renderer, &rect);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
    }
};
