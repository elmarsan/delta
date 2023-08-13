#pragma once

#include "../delta/Game.h"
#include "ECS.h"
#include "SDL.h"
#include "SpriteComponent.h"
#include "TextureManager.h"
#include "TransformComponent.h"

namespace engine
{

enum TileID
{
    GRASS,
    WATER,
};

namespace component
{

    class Tile: public Component
    {
      public:
        engine::component::Sprite* sprite;
        SDL_Texture* texture;
        SDL_Rect rect;
        TileID ID;

        Tile() = default;
        Tile(int x, int y, int w, int h, TileID id)
        {
            rect = SDL_Rect { x, y, w, h };
            ID = id;

            switch (ID)
            {
                case GRASS: texture = TextureManager::load("data/32grass.png"); break;
                case WATER: texture = TextureManager::load("data/32water.png"); break;
            }
        }

        void init() override
        {
            // transform = &entity->addComponent<TransformComponent>(rect.x, rect.y, rect.w, rect.h, 1);
            // sprite = &entity->addComponent<SpriteComponent>(texture);
        }
    };
}; // namespace component
}; // namespace engine
