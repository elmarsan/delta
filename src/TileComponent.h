// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Asset.h"
#include "ECS.h"
#include "Game.h"
#include "System.h"
#include "WindowManager.h"
#include "absl/log/log.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <memory>

class TileComponent: public Component
{
  private:
    Rect src;
    Rect dst;
    // SDL_FRect src, dst;
    Tile tile;
    std::shared_ptr<Tileset> tileset;
    std::shared_ptr<Texture> texture;
    Point2 tilePoint2;

  public:
    Point2 gridPoint2;

    TileComponent(Point2 gridPos, const Tile& tile): tile(tile), tilePoint2(gridPos), gridPoint2(gridPos)
    {
        src = Rect { tile.point2.x, tile.point2.y, 16, 16 };
        dst = Rect { gridPos.x, gridPos.y, 44, 44 };
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
            src = Rect { tilePoint2.x, tilePoint2.y, tile.size2.w, tile.size2.h };
        }
        auto camPos = System::actorSystem().getCameraPos();
        dst = Rect { gridPoint2.x - camPos.x, gridPoint2.y - camPos.y, 44, 44 };
    }

    void draw() override
    {
        System::windowSystem().renderTexture(*texture.get(), *src, const SDL_FRect *dst, const SDL_RendererFlip flip)
        // WindowManager::Instance()->renderTexture(texture, &src, &dst, SDL_FLIP_NONE);
#ifdef DEBUG
        System::windowSystem().renderRect();
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0xff, 0, 0);
        SDL_RenderDrawRectF(WindowManager::Instance()->renderer, &dst);
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 0, 0);
#endif
    }
};
