// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "math/Polygon.h"
#include "math/Rect.h"
#include "math/Vec2.h"

#include <SDL2/SDL_video.h>

class WindowManager
{
  public:
    virtual ~WindowManager() = default;

    [[nodiscard]] virtual Vec2 getSize() const = 0;
};

class SDLWindowManager: public WindowManager
{
  public:
    SDLWindowManager(SDL_Window* window): window(window) {}
    SDLWindowManager(const SDLWindowManager&) = delete;
    SDLWindowManager(SDLWindowManager&&) = delete;

    [[nodiscard]] inline Vec2 getSize() const override
    {
        int w;
        int h;
        SDL_GetWindowSize(window, &w, &h);
        return Vec2(w, h);
    }

  private:
    SDL_Window* window;
};
