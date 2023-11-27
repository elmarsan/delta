// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Asset.h"

#include <absl/status/status.h>
#include <string>

class MapManager
{
  public:
    static absl::Status draw(const AssetID mapID, const Point2 gridPos);
    static int getTileID(std::vector<std::tuple<int, std::string>> tilesets, int tileGID);
    static Tile getTileV2(std::vector<std::shared_ptr<Tileset>> tilesets, int tileGID, int mapTilesetFirstGID);
};
