#pragma once

#include "Vector2D.h"
#include "delta/Tileset.h"

class TileManager
{
  public:
    static void addTile(Vector2D gridPos, Texture texture);
    static void addAnimatedTile(Vector2D gridPos, std::string tilesetID, std::vector<int> tileIDs, int speed);
};
