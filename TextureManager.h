#pragma once

#include "SDL_pixels.h"

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class TextureManager
{
  public:
    static SDL_Texture* load(std::string path, SDL_Renderer* renderer);
    static SDL_Texture* load(std::string path, SDL_Renderer* renderer, SDL_Color colorMod);
};
