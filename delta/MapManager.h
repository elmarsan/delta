#pragma once

#include "Asset.h"

#include <absl/status/status.h>
#include <map>
#include <string>

class MapManager
{
  public:
    static absl::Status draw(const AssetID mapID, const Vector2D gridPos);
    static int getTileID(std::vector<std::tuple<int, std::string>> tilesets, int tileGID);
    static Tile getTileV2(std::vector<std::shared_ptr<Tileset>> tilesets, int tileGID, int mapTilesetFirstGID);
};
