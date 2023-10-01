#pragma once

#include "Vector2D.h"
#include "WorldManager.h"
#include "engine/Asset.h"

class TileManager
{
  public:
    static void addTile(Vector2D gridPos, Tile tile);
    static void destroyMapTiles(WorldMap map);
};
