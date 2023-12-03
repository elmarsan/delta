// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Asset.h"
#include "SDL_error.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "absl/log/log.h"
#include "absl/status/statusor.h"
#include "math/Polygon.h"
#include "math/Rect.h"

#include <filesystem>
#include <memory>

class Engine;

class RenderManager
{
  public:
    virtual ~RenderManager() {}
    virtual void addRect(const Rect& rect) = 0;
    virtual void addPolygon(const Polygon& polygon) = 0;
    virtual void addTexture(Texture* texture,
                            const Rect* src,
                            const Rect* dst,
                            const SDL_RendererFlip flip) = 0;

    virtual void render() = 0;
    virtual void clear() = 0;

    // The ownership of Texture belongs to the RenderManager.
    [[nodiscard]] virtual Texture* getTexture(const std::string id) = 0;
};

class SDLRenderManager: public RenderManager
{
  public:
    SDLRenderManager(SDL_Renderer* renderer): renderer(renderer) {}
    ~SDLRenderManager() { SDL_DestroyRenderer(renderer); }
    SDLRenderManager(const SDLRenderManager&) = delete;
    SDLRenderManager(SDLRenderManager&&) = delete;

    void addTexture(Texture* texture,
                    const Rect* src,
                    const Rect* dst,
                    const SDL_RendererFlip flip) override;

    void addRect(const Rect& rect) override;
    void addPolygon(const Polygon& polygon) override;

    inline void render() override { SDL_RenderPresent(renderer); }
    inline void clear() override { SDL_RenderClear(renderer); }

    // The ownership of Texture belongs to the SDLRenderManager.
    [[nodiscard]] Texture* getTexture(const std::string id) override;

  private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
};
