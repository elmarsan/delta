#pragma once

#include "ECS.h"
#include "SDL_render.h"
#include "TextureManager.h"
#include "delta/Game.h"
#include "engine/Texture.h"

#include <SDL.h>

class TileComponent: public Component
{
  public:
    Texture texture;
    SDL_Rect src, dst;

    TileComponent() = default;
    TileComponent(Vector2D position, Texture texture): texture(texture)
    {
        src = SDL_Rect { texture.pos.x, texture.pos.y, 32, 32 };
        dst = SDL_Rect { position.x, position.y, 44, 44 };
    }

    void draw() override
    {
        TextureManager::draw(texture.sdlTexture, &src, &dst, texture.flip);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0xff, 0, 0);
        SDL_RenderDrawRect(Game::renderer, &dst);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
    }
};
