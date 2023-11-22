// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "math/Vec2.h"
#include "WorldManager.h"
#include "Asset.h"

class TileManager
{
  public:
    static void addTile(Point2 point2, Tile tile);
    static void destroyMapTiles(WorldMap map);
    static Point2 findClosestPoint2(Point2 point2);

  private:
    static int findClosestNumber(int num);
};
