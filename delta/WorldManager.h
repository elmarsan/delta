#pragma once

#include "Asset.h"
#include "absl/status/status.h"
#include "absl/strings/str_format.h"

#include <map>
#include <vector>

using MapID = AssetID;

class WorldMap
{
  public:
    WorldMap() {};
    WorldMap(std::string filename, int x, int y, int w, int h);

    double distance(const WorldMap& map) const;
    bool isAdjacent(const WorldMap& map) const;
    MapID getID() const;
    SDL_Rect getRect() const;

  private:
    MapID mapID;
    SDL_Rect rect;
    int x;
    int y;
    int w;
    int h;
};

class WorldManager
{
  public:
    WorldManager() = default;
    ~WorldManager() = default;

    static std::shared_ptr<WorldManager> Instance();

    MapID mapFromPos(Vector2D pos);
    std::vector<WorldMap> adjacentMapIDs(MapID mapID);

  private:
    WorldManager(const WorldManager&) = delete;
    void operator=(const WorldManager&) = delete;

    static absl::Status init();

    static std::shared_ptr<WorldManager> instance;
    std::vector<WorldMap> maps;
};
