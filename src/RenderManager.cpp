// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "RenderManager.h"

#include "SDL_error.h"
#include "Engine.h"
#include "absl/log/log.h"

void SDLRenderManager::addTexture(Texture* texture,
                                  const Rect* src,
                                  const Rect* dst,
                                  const SDL_RendererFlip flip)
{
    if (texture == nullptr) 
    {
        return;
    }
    SDL_Rect srcRect {
        static_cast<int>(src->x), static_cast<int>(src->y), static_cast<int>(src->w), static_cast<int>(src->h)
    };
    SDL_FRect dstRect;
    if (dst != nullptr)
    {
        dstRect.x = dst->x;
        dstRect.y = dst->y;
        dstRect.w = dst->w;
        dstRect.h = dst->h;
    }
    SDL_RenderCopyExF(renderer, texture->sdlTexture, &srcRect, &dstRect, 0, NULL, flip);
}

void SDLRenderManager::addRect(const Rect& rect)
{
    auto camPos = Engine::actor().getCameraPos();
    auto camSize = Engine::actor().getCameraSize();

    SDL_FRect sdlRect { rect.x - camPos.x, rect.y - camPos.y + 44, camSize.w, camSize.h };
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0);
    SDL_RenderDrawRectF(renderer, &sdlRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void SDLRenderManager::addPolygon(const Polygon& polygon)
{
    auto camPos = Engine::actor().getCameraPos();

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

Texture* SDLRenderManager::getTexture(const std::string id)
{
    auto it = textures.find(id);
    if (it != textures.end())
    {
        return it->second.get();
    }

    std::string path = absl::StrFormat("data/assets/%s.png", id);
    if (!std::filesystem::exists(path))
    {
        return nullptr;
    }

    LOG(INFO) << absl::StrFormat("Loading Texture: %s", id);
    SDL_Surface* surface = IMG_Load(path.c_str());

    SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    textures[id] = std::make_unique<Texture>(id, sdlTexture);
    return textures[id].get(); 
}
