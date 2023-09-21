#pragma once

#include "Asset.h"
#include "ECS.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "delta/Game.h"

#include <SDL2/SDL_timer.h>
#include <memory>

class TileComponent: public Component
{
  private:
    SDL_Rect src, dst;
    Tile tile;
    std::shared_ptr<Tileset> tileset;
    std::shared_ptr<Texture> texture;
    Vector2D pos;

  public:
    TileComponent(Vector2D gridPos, const Tile& tile): tile(tile), pos(gridPos)
    {
        src = SDL_Rect { tile.pos.x, tile.pos.y, 16, 16 };
        dst = SDL_Rect { gridPos.x, gridPos.y, 44, 44 };
    }

    int zindex() { return tile.zindex; }

    void init() override
    {
        auto res = Game::assetManager->getOrLoad<Texture>(tile.textureID);
        LOG_IF(ERROR, !res.ok()) << res.status().message();
        if (res.ok())
            texture = res.value();

        if (tile.isAnimated())
        {
            auto res = Game::assetManager->getOrLoad<Tileset>(tile.tilesetID);
            LOG_IF(ERROR, !res.ok()) << res.status().message();
            if (res.ok())
            {
                tileset = res.value();
            }
        }
    }

    void update() override
    {
        if (tile.isAnimated())
        {
            int numFrames = tile.numFrames();
            int speed = tile.animSpeed();
            int tileIndex = static_cast<int>((SDL_GetTicks() / speed) % numFrames);
            auto frames = std::get<FrameIDs>(tile.frames);
            pos = tileset->getTile(frames[tileIndex]).pos;
            src = SDL_Rect { pos.x, pos.y, tile.width, tile.height };
        }

        dst = SDL_Rect { pos.x - WindowManager::Instance()->camera.x,
                         pos.y - WindowManager::Instance()->camera.y,
                         44,
                         44 };
    }

    void draw() override
    {
        WindowManager::Instance()->renderTexture(texture, &src, &dst);
#ifdef DEBUG
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0xff, 0, 0);
        SDL_RenderDrawRect(WindowManager::Instance()->renderer, &dst);
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 0, 0);
#endif
    }
};
