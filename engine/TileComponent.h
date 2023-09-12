#pragma once

#include "ECS.h"
#include "TextureManager.h"
#include "delta/Game.h"
#include "engine/Texture.h"

class TileComponent: public Component
{
  public:
    Texture texture;
    SDL_Rect src, dst;
    Vector2D pos;

    TileComponent() = default;
    TileComponent(Vector2D pos, Texture texture): texture(texture), pos(pos)
    {
        src = SDL_Rect { texture.pos.x, texture.pos.y, 16, 16 };
        dst = SDL_Rect { pos.x, pos.y, 44, 44 };
    }

    void draw() override
    {
        TextureManager::draw(texture.sdlTexture, &src, &dst, texture.flip);
#ifdef DEBUG
        SDL_SetRenderDrawColor(Game::renderer, 0, 0xff, 0, 0);
        SDL_RenderDrawRect(Game::renderer, &dst);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
#endif
    }
};
