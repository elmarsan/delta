// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Asset.h"
#include "math/Vec2.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"

#include <map>
#include <vector>

using MapID = AssetID;

class WorldMap
{
  public:
    WorldMap() {};
    WorldMap(std::string filename, float x, float y, float w, float h);

    double distance(const WorldMap& map) const;
    bool isAdjacent(const WorldMap& map) const;
    bool pointIn(const Point2& point) const;
    MapID getID() const;
    SDL_FRect getRect() const;
    Point2 getWorldPos() const;

  private:
    Point2 point2;
    Size2 size2;
    MapID mapID;
    SDL_FRect rect;
};

class WorldManager
{
  public:
    WorldManager() = default;
    ~WorldManager() = default;

    static std::shared_ptr<WorldManager> Instance();

    absl::Status setCurrentMap(MapID mapID);
    absl::StatusOr<WorldMap> findMapFromPos(Point2 pos) const;
    MapID getCurrentMapID() const;

  private:
    WorldManager(const WorldManager&) = delete;
    void operator=(const WorldManager&) = delete;

    static absl::Status init();
    absl::Status loadAdjMaps();
    absl::Status destroyNonAdjMaps();
    absl::StatusOr<WorldMap> findMapFromID(MapID mapID);

    static std::shared_ptr<WorldManager> instance;
    std::vector<WorldMap> maps;
    WorldMap currentMap;
    std::vector<WorldMap> adjMaps;
    std::vector<WorldMap> nonAdjMaps;
};
