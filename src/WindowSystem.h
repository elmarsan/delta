// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Asset.h"
#include "math/Polygon.h"
#include "math/Rect.h"
#include "math/Vec2.h"

class System;

class WindowSystem
{
  public:
    virtual ~WindowSystem() {}
    // TODO: use math rects
    virtual void renderTexture(const Texture& texture,
                               const Rect* src,
                               const Rect* dst,
                               const SDL_RendererFlip flip) const = 0;

    virtual void renderRect(const Rect& rect) const = 0;
    virtual void renderPolygon(const Polygon& polygon) const = 0;

    [[nodiscard]] virtual Vec2 getSize() const = 0;
};

class SDLWindowSystem: public WindowSystem
{
  public:
    SDLWindowSystem(int width, int height);
    SDLWindowSystem(const SDLWindowSystem&) = delete;
    SDLWindowSystem(SDLWindowSystem&&) = delete;

    void renderTexture(const Texture& texture,
                       const Rect* src,
                       const Rect* dst,
                       const SDL_RendererFlip flip) const override;

    virtual void renderRect(const Rect& rect) const override;
    virtual void renderPolygon(const Polygon& polygon) const override;

    [[nodiscard]] Vec2 getSize() const override;

  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};
