#pragma once

#include "ECS.h"
#include "SDL.h"
#include "SpriteComponent.h"
#include "TextureManager.h"
#include "TransformComponent.h"

enum TileID
{
    GRASS,
    WATER,
};

class TileComponent: public Component
{
  public:
    Sprite* sprite;
    SDL_Texture* texture;
    SDL_Rect rect;
    TileID ID;

    TileComponent() = default;
    TileComponent(int x, int y, int w, int h, TileID id)
    {
        rect = SDL_Rect { x, y, w, h };
        ID = id;

        switch (ID)
        {
            case GRASS: texture = TextureManager::load("data/32grass.png"); break;
            case WATER: texture = TextureManager::load("data/32water.png"); break;
        }
    }
};
