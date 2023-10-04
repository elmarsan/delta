#pragma once

#include "Vector2.h"
#include "WorldManager.h"
#include "engine/Asset.h"

class TileManager
{
  public:
    static void addTile(Point2 point2, Tile tile);
    static void destroyMapTiles(WorldMap map);
};
