#pragma once

#include <SDL.h>
#include <string>

class TextureManager
{
  public:
    static SDL_Texture* load(std::string path, SDL_Color* colorMod = nullptr);
    static void draw(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect, SDL_RendererFlip renderFlip);
};
