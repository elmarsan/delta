#pragma once

#include "Game.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "Vector2.h"

class TeleportComponent: public Component
{
  public:
    SDL_Rect rect;
    Point2 destPoint2;
    std::string mapID;
    std::string level;

    TeleportComponent(Vector2 source, Vector2 destiny, std::string mapID, std::string level = ""):
        destPoint2(destiny), mapID(mapID), level(level)
    {
        rect = SDL_Rect { source.x, source.y, 44, 44 };
    }
};