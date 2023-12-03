// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Asset.h"
#include "ECS.h"
#include "WorldManager.h"
#include "math/Vec2.h"

extern Manager manager;

class TerrainManager
{
  public:
    virtual ~TerrainManager() = default;

    virtual void addCell(Vec2 pos, Tile tile) = 0;
    virtual void destroy(WorldMap map) = 0;
};

class DeltaTerrainManager: public TerrainManager
{
  public:
    DeltaTerrainManager() {}

    void addCell(Vec2 pos, Tile tile) override;
    void destroy(WorldMap map) override;
};
