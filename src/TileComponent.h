#pragma once

#include "Asset.h"
#include "ECS.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "Game.h"

#include <SDL2/SDL_timer.h>
#include <memory>

class TileComponent: public Component
{
  private:
    SDL_Rect src, dst;
    Tile tile;
    std::shared_ptr<Tileset> tileset;
    std::shared_ptr<Texture> texture;
    Point2 tilePoint2;

  public:
    Point2 gridPoint2;

    TileComponent(Vector2 gridPos, const Tile& tile): tile(tile), tilePoint2(gridPos), gridPoint2(gridPos)
    {
        src = SDL_Rect { tile.point2.x, tile.point2.y, 16, 16 };
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
                tileset = res.value();
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
            tilePoint2 = tileset->getTile(frames[tileIndex]).point2;
            src = SDL_Rect { tilePoint2.x, tilePoint2.y, tile.size2.w, tile.size2.h};
        }

        dst = SDL_Rect { gridPoint2.x - WindowManager::Instance()->camera.x,
                         gridPoint2.y - WindowManager::Instance()->camera.y,
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
