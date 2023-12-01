// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "WindowManager.h"

#include "absl/log/log.h"
#include "absl/status/status.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <memory>

const int defaultWidth = 640;
const int defaultHeight = 480;

std::shared_ptr<WindowManager> WindowManager::instance = nullptr;

std::shared_ptr<WindowManager> WindowManager::Instance()
{
    if (instance == nullptr)
    {
        auto res = init(defaultWidth, defaultHeight);
        LOG_IF(FATAL, !res.ok()) << res.message();
    }

    return instance;
}

absl::Status WindowManager::init(float width, float height)
{
    DLOG(INFO) << "Initializing WindowManager...";

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return absl::InternalError("Unable to init SDL");
    if (IMG_Init(IMG_INIT_PNG) == 0)
        return absl::InternalError("Unable to init SDL Image ");

    instance = std::make_shared<WindowManager>();
    instance->w = width;
    instance->h = height;
    instance->camera = { 0, 0, width, height };
    instance->window = SDL_CreateWindow(
        "Delta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_SOFTWARE);

    return absl::OkStatus();
}

float WindowManager::width()
{
    return w;
}

float WindowManager::height()
{
    return h;
}

// void WindowManager::setWidth(int w)
// {
//     w = w;
//     camera.w = w;
// }

// void WindowManager::setHeight(int h)
// {
//     h = h;
//     camera.h = h;
// }

void WindowManager::renderTexture(std::shared_ptr<Texture> texture,
                                  SDL_FRect* src,
                                  SDL_FRect* dst,
                                  SDL_RendererFlip flip)
{
    SDL_Rect srcRect {
        static_cast<int>(src->x), static_cast<int>(src->y), static_cast<int>(src->w), static_cast<int>(src->h)
    };
    SDL_RenderCopyExF(
        WindowManager::Instance()->renderer, texture->sdlTexture, &srcRect, dst, 0, NULL, flip);
}

WindowManager::~WindowManager()
{
    SDL_DestroyRenderer(instance->renderer);
    SDL_DestroyWindow(instance->window);
}
