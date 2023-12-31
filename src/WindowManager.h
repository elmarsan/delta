// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Asset.h"
#include "math/Vec2.h"
#include "absl/status/status.h"

#include <SDL2/SDL_render.h>
#include <memory>

class WindowManager
{
  public:
    WindowManager() = default;
    ~WindowManager();

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_FRect camera;

    static std::shared_ptr<WindowManager> Instance();
    static absl::Status init(float width, float height);

    float width();
    float height();
    void setWidth(int w);
    void setHeight(int h);

    void renderTexture(std::shared_ptr<Texture> texture, SDL_FRect* src, SDL_FRect* dst, SDL_RendererFlip flip);

  private:
    WindowManager(const WindowManager&) = delete;
    void operator=(const WindowManager&) = delete;

    static std::shared_ptr<WindowManager> instance;
    float w;
    float h;
};
