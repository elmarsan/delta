#pragma once

#include "ECS.h"
#include "TextureManager.h"
#include "delta/Game.h"
#include "delta/TilesetManager.h"
#include "engine/Texture.h"

#include <SDL.h>

class AnimatedTileComponent: public Component
{
  private:
    Tileset tileset;
    SDL_Rect src, dst;
    Vector2D gridPos;
    Vector2D framePos;

    std::string tilesetID;
    std::vector<int> tileIDs;
    int speed;

    int numFrames() { return tileIDs.size(); }

  public:
    AnimatedTileComponent(Vector2D gridPos, std::string tilesetID, std::vector<int> tileIDs, int speed):
        gridPos(gridPos), tilesetID(tilesetID), tileIDs(tileIDs), speed(speed)
    {
        dst = SDL_Rect { gridPos.x, gridPos.y, 44, 44 };
    }

    void init() override { tileset = TilesetManager::getTileset(tilesetID); }

    void update() override
    {
        int tileIndex = static_cast<int>((SDL_GetTicks() / speed) % numFrames());
        framePos = tileset.getTile(tileIDs[tileIndex]).pos;
    }

    void draw() override
    {
        src = SDL_Rect { framePos.x, framePos.y, tileset.tileWidth, tileset.tileHeight };
        TextureManager::draw(tileset.texture, &src, &dst, SDL_FLIP_NONE);
#ifdef DEBUG
        SDL_SetRenderDrawColor(Game::renderer, 0, 0xff, 0, 0);
        SDL_RenderDrawRect(Game::renderer, &dst);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
#endif
    }
};
