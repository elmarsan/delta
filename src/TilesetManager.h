// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Asset.h"
#include "absl/status/statusor.h"

#include <string>

class TilesetManager
{
  public:
    ~TilesetManager() = default;

    [[nodiscard]] virtual Tileset* get(const std::string id) = 0;
    [[nodiscard]] virtual Tile getTile(const std::vector<std::string> tilesetIds,
                                                       int tileGid,
                                                       int offset) = 0;
};

class TiledTilesetManager: public TilesetManager
{
  public:
    TiledTilesetManager() {}

    [[nodiscard]] Tileset* get(const std::string id) override;
    [[nodiscard]] Tile getTile(const std::vector<std::string> tilesetIds,
                                               int tileGid,
                                               int offset) override;

  private:
    std::unordered_map<std::string, std::unique_ptr<Tileset>> tilesets;
};
