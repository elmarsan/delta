#pragma once

#include "engine/Animation.h"
#include "engine/Texture.h"

#include <SDL.h>
#include <map>
#include <vector>
#include <string>

struct Tile
{
    SDL_Texture* texture;
    std::string tilesetID;
    Vector2D pos;
    std::tuple<std::vector<int>, int> frames;

    bool isAnimated();
};

struct Tileset
{
    SDL_Texture* texture;
    std::string ID;
    std::string image;
    std::map<int, std::tuple<std::vector<int>, int>> tileFrames;
    int columns;
    int tileWidth, tileHeight;
    int count;
    int imageWidth, imageHeight;
    int margin;
    int spacing;

    Tile getTile(const int tileID);
};
