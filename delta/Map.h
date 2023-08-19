#pragma once

#include <SDL.h>
#include <string>

class Map
{
  public:
    int w;
    int h;
    int tileWidth;
    int tileHeight;

    Map();
    ~Map();

    void load(std::string path);
    void addTile(int x, int y, int tileID, bool hasCollider);

  private:
    SDL_Texture* grassTexture;
    SDL_Texture* waterTexture;
};
