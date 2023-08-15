#pragma once

#include "SDL.h"
class Map
{
  public:
    SDL_Texture* grassTexture;
    SDL_Texture* waterTexture;

    Map();
    ~Map();

    void load();
    void draw();

  private:
    SDL_Rect src;
    SDL_Rect dst;

    int map[20][25];
};
