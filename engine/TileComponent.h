#pragma once

#include "ECS.h"
#include "TextureManager.h"
#include "delta/Game.h"

#include <SDL.h>

class TileComponent: public Component
{
  public:
    SDL_Texture* texture;
    SDL_Rect src, dst;
    int w = 44;
    int h = 44;

    TileComponent() = default;
    TileComponent(int x, int y, SDL_Texture* texture): texture(texture)
    {
        src = SDL_Rect { 0, 0, 32, 32 };
        dst = SDL_Rect { x, y, 44, 44 };
    }

    void draw() override
    {
        TextureManager::draw(texture, &src, &dst, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0xff, 0, 0);
        SDL_RenderDrawRect(Game::renderer, &dst);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
    }
};
