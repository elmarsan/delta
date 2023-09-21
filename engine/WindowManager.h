#pragma once

#include "Asset.h"
#include "Vector2D.h"
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
    SDL_Rect camera;

    static std::shared_ptr<WindowManager> Instance();
    static absl::Status init(int width, int height);

    int width();
    int height();
    void setWidth(int w);
    void setHeight(int h);
    
    void renderTexture(std::shared_ptr<Texture> texture, SDL_Rect* src, SDL_Rect* dst);

  private:
    WindowManager(const WindowManager&) = delete;
    void operator=(const WindowManager&) = delete;

    static std::shared_ptr<WindowManager> instance;
    int w;
    int h;
};
