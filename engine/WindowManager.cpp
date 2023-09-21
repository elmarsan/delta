#include "WindowManager.h"

#include "absl/log/log.h"
#include "absl/status/status.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

absl::Status WindowManager::init(int width, int height)
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

int WindowManager::width()
{
    return w;
}

int WindowManager::height()
{
    return h;
}

void WindowManager::setWidth(int w)
{
    w = w;
    camera.w = w;
}

void WindowManager::setHeight(int h)
{
    h = h;
    camera.h = h;
}

void WindowManager::renderTexture(std::shared_ptr<Texture> texture, SDL_Rect* src, SDL_Rect* dst)
{
    SDL_RenderCopyEx(
        WindowManager::Instance()->renderer, texture->sdlTexture, src, dst, 0, NULL, texture->flip);
}

WindowManager::~WindowManager()
{
    SDL_DestroyRenderer(instance->renderer);
    SDL_DestroyWindow(instance->window);
}
