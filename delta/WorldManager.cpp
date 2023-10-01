#include "WorldManager.h"

#include "absl/log/log.h"
#include "absl/status/status.h"
#include "nlohmann/json.hpp"

#include <SDL2/SDL_rect.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <cmath>

WorldMap::WorldMap(std::string filename, int x, int y, int w, int h): x(x), y(y), w(w), h(h)
{
    size_t lastDotPos = filename.find_last_of('.');
    if (lastDotPos != std::string::npos)
        mapID = filename.substr(0, lastDotPos);

    rect = { x, y, w, h };
}

double WorldMap::distance(const WorldMap& map) const
{
    return std::sqrt(std::pow(map.x - x, 2) + std::pow(map.y - y, 2));
}

bool WorldMap::isAdjacent(const WorldMap& map) const
{
    if (map.x - map.w == x)
        return true;
    if (map.x + map.w == x)
        return true;
    if (map.y + map.h == y)
        return true;
    if (map.y - map.h == y)
        return true;

    return false;
}

MapID WorldMap::getID() const
{
    return mapID;
}

SDL_Rect WorldMap::getRect() const
{
    return rect;
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
    LOG(INFO) << "Initialising WorldManager";
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

MapID WorldManager::mapFromPos(Vector2D pos)
{
    SDL_Point point = { pos.x, pos.y };
    for (auto& map: maps)
    {
        auto rect = map.getRect();
        if (SDL_PointInRect(&point, &rect))
            return map.getID();
    }

    LOG(ERROR) << "Unable to find map for position: " << pos;
    return "";
}

std::vector<WorldMap> WorldManager::adjacentMapIDs(MapID mapID)
{
    WorldMap targetMap {};
    std::vector<WorldMap> otherMaps;

    for (auto& m: maps)
    {
        if (m.getID() == mapID)
            targetMap = m;
        else
            otherMaps.push_back(m);
    }

    std::sort(otherMaps.begin(), otherMaps.end(), [&targetMap](const WorldMap& a, const WorldMap& b) {
        return a.distance(targetMap) < b.distance(targetMap);
    });

    auto adjacentMaps = std::vector<WorldMap>();
    for (auto it = otherMaps.begin(); it != otherMaps.begin() + 3; ++it)
    {
        if (targetMap.isAdjacent(*it))
            adjacentMaps.emplace_back(*it);
    }

    return adjacentMaps;
}
