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
    virtual ~MapManager() = default;

    [[nodiscard]] virtual Map* get(const std::string id) = 0;
    [[nodiscard]] virtual absl::Status draw(const AssetID mapID, const Point2 gridPos) = 0;
};

class DeltaMapManager: public MapManager
{
  public:
    DeltaMapManager() {}

    [[nodiscard]] Map* get(const std::string id) override;
    [[nodiscard]] absl::Status draw(const AssetID mapID, const Point2 gridPos) override;

  private:
    std::unordered_map<std::string, std::unique_ptr<Map>> maps;
};
