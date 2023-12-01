// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "WindowSystem.h"

#include "System.h"
#include "math/Polygon.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

SDLWindowSystem::SDLWindowSystem(int width, int height)
{
    DLOG(INFO) << "Initializing SDLWindowSystem...";

    LOG_IF(FATAL, SDL_Init(SDL_INIT_VIDEO) != 0) << "Unable to init sdl video";
    LOG_IF(FATAL, IMG_Init(IMG_INIT_PNG) == 0) << "Unable to init sdl image";

    window = SDL_CreateWindow(
        "Delta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    LOG_IF(FATAL, window == nullptr) << "Unable to create window";

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    LOG_IF(FATAL, renderer == nullptr) << "Unable to create renderer";
}

void SDLWindowSystem::renderTexture(const Texture& texture,
                                    const Rect* src,
                                    const Rect* dst,
                                    const SDL_RendererFlip flip) const
{
    SDL_Rect srcRect {
        static_cast<int>(src->x), static_cast<int>(src->y), static_cast<int>(src->w), static_cast<int>(src->h)
    };
    SDL_FRect *dstRect  = nullptr;
    if (dst != nullptr) {
        dstRect->x = dst->x;
        dstRect->y = dst->y;
        dstRect->w = dst->w;
        dstRect->h = dst->h;
    }
    SDL_RenderCopyExF(renderer, texture.sdlTexture, &srcRect, dstRect, 0, NULL, flip);
}

void SDLWindowSystem::renderRect(const Rect& rect) const
{
    auto camPos = System::actorSystem().getCameraPos();
    auto camSize = System::actorSystem().getCameraSize();

    SDL_FRect sdlRect { rect.x - camPos.x, rect.y - camPos.y + 44, camSize.w, camSize.h };
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0);
    SDL_RenderDrawRectF(renderer, &sdlRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void SDLWindowSystem::renderPolygon(const Polygon& polygon) const
{
    auto camPos = System::actorSystem().getCameraPos();
    auto camSize = System::actorSystem().getCameraSize();

    std::vector<SDL_FPoint> points;
    for (const auto& v: polygon.vertex)
    {
        points.push_back(SDL_FPoint {
            v.x - camPos.x,
            v.y - camPos.y + 44,
        });
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLinesF(renderer, points.data(), points.size());
    SDL_RenderDrawLine(renderer, points.back().x, points.back().y, points.front().x, points.front().y);
}

[[nodiscard]] Vec2 SDLWindowSystem::getSize() const
{
    int w;
    int h;
    SDL_GetWindowSize(window, &w, &h);
    return Vec2(w, h);
}
