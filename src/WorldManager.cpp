// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "WorldManager.h"

#include "MapManager.h"
#include "Engine.h"
#include "TileManager.h"
#include "math/Vec2.h"
#include "absl/log/log.h"
#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include "nlohmann/json.hpp"

#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>

WorldMap::WorldMap(std::string filename, float x, float y, float w, float h)
{
    size_t lastDotPos = filename.find_last_of('.');
    if (lastDotPos != std::string::npos)
        mapID = filename.substr(0, lastDotPos);

    point2 = Point2(x, y);
    size2 = Size2(x, y);
    rect = { x, y, w, h };
}

double WorldMap::distance(const WorldMap& map) const
{
    return map.point2.distanceTo(point2);
}

bool WorldMap::isAdjacent(const WorldMap& map) const
{
    if (map.point2.x - map.size2.w == point2.x)
        return true;
    if (map.point2.x + map.size2.w == point2.x)
        return true;
    if (map.point2.y + map.size2.h == point2.y)
        return true;
    if (map.point2.y - map.size2.h == point2.y)
        return true;

    return false;
}

bool WorldMap::pointIn(const Point2& point) const
{
    SDL_FPoint p { point.x, point.y };
    return SDL_PointInFRect(&p, &rect) == SDL_TRUE;
}

MapID WorldMap::getID() const
{
    return mapID;
}

SDL_FRect WorldMap::getRect() const
{
    return rect;
}

Point2 WorldMap::getWorldPos() const
{
    return point2;
}
std::shared_ptr<WorldManager> WorldManager::instance = nullptr;

std::shared_ptr<WorldManager> WorldManager::Instance()
{
    if (instance == nullptr)
    {
        auto res = init();
        LOG_IF(FATAL, !res.ok()) << res.message();
    }

    return instance;
}

absl::Status WorldManager::init()
{
    std::string path = "data/maps/hoenn.world";
    if (!std::filesystem::exists(path))
        return absl::NotFoundError("World not found");

    instance = std::make_shared<WorldManager>();

    std::ifstream f(path);
    nlohmann::json data = nlohmann::json::parse(f);

    for (auto& map: data["maps"])
    {
        std::string filename = map["fileName"];
        int x = map["x"];
        int y = map["y"];
        int w = map["width"];
        int h = map["height"];

        WorldMap worldMap(filename, (x / 16) * 44, (y / 16) * 44, (w / 16) * 44, (h / 16) * 44);
        instance->maps.emplace_back(worldMap);
    }

    return absl::OkStatus();
}

absl::Status WorldManager::setCurrentMap(MapID mapID)
{
    LOG(INFO) << absl::StrFormat("Setting current map to: %s", mapID);
    auto findMapRes = findMapFromID(mapID);
    if (!findMapRes.ok())
        return findMapRes.status();

    currentMap = findMapRes.value();
   
    auto oldAdjMaps = adjMaps;
    auto loadRes = loadAdjMaps();
    if (!loadRes.ok())
        return loadRes;

    auto destroyRes = destroyNonAdjMaps();
    if (!destroyRes.ok())
        return destroyRes;

    auto loadedMaps = oldAdjMaps;
    loadedMaps.emplace_back(currentMap);
    auto maps = adjMaps;
    maps.emplace_back(currentMap);
    for (auto &m: maps) 
    {
        bool alreadyLoaded = false;
        for (auto& oldMap: oldAdjMaps)
        {
            if (oldMap.getID() == m.getID())
                alreadyLoaded = true;
        }

        if (!alreadyLoaded)
        {
            auto drawRes = Engine::map().draw(m.getID(), m.getWorldPos());
            LOG_IF(ERROR, !drawRes.ok()) << drawRes.message(); 
        }
    }

    return absl::OkStatus();
}


absl::StatusOr<WorldMap> WorldManager::findMapFromPos(Point2 pos) const
{
    SDL_FPoint point = { pos.x, pos.y };
    for (auto& map: maps)
    {
        auto rect = map.getRect();
        if (SDL_PointInFRect(&point, &rect))
            return map;
    }

    return absl::NotFoundError(absl::StrFormat("Unable to get map from position: %v", pos));
}


MapID WorldManager::getCurrentMapID() const
{
    return currentMap.getID();
}

absl::Status WorldManager::loadAdjMaps()
{
    LOG(INFO) << absl::StrFormat("Loading adj maps to: %s", currentMap.getID());
    std::vector<WorldMap> otherMaps;
    for (auto& m: maps)
    {
        if (m.getID() != currentMap.getID())
            otherMaps.push_back(m);
    }

    std::sort(otherMaps.begin(), otherMaps.end(), [this](const WorldMap& a, const WorldMap& b) {
        return a.distance(currentMap) < b.distance(currentMap);
    });

    adjMaps.clear();
    nonAdjMaps.clear();
    for (auto it = otherMaps.begin(); it != otherMaps.begin() + 3; ++it)
    {
        if (currentMap.isAdjacent(*it))
            adjMaps.emplace_back(*it);
        else
         nonAdjMaps.emplace_back(*it);
    }

    return absl::OkStatus();
}

absl::Status WorldManager::destroyNonAdjMaps()
{
    if (nonAdjMaps.size() > 0) 
        return absl::OkStatus();

    LOG(INFO) << absl::StrFormat("Destroying non adj maps to: %s", currentMap.getID());
    for (auto& m: nonAdjMaps)
    {
       Engine::terrain().destroy(m);
    }

    return absl::OkStatus();
}

absl::StatusOr<WorldMap> WorldManager::findMapFromID(MapID mapID)
{
    for (auto& map: maps)
    {
         if (map.getID() == mapID)
            return map;
    }

    return absl::NotFoundError(absl::StrFormat("Unable to find map: %s", mapID));
}
