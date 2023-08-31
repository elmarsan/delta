#include "MapManager.h"

#include "TilesetManager.h"
#include "engine/TileManager.h"
#include "json.hpp"

#include <absl/status/status.h>
#include <absl/strings/str_format.h>
#include <filesystem>
#include <fstream>

std::map<std::string, Map> MapManager::maps = std::map<std::string, Map>();

absl::Status MapManager::loadJSON(const std::string mapID)
{
    std::string path = absl::StrFormat("data/maps/export/%s.json", mapID);
    if (!std::filesystem::exists(path))
    {
        return absl::NotFoundError(absl::StrFormat("Map: %s not found", mapID));
    }

    std::ifstream file(path);
    if (!file.is_open())
    {
        return absl::InternalError("Unable to open the map file");
    }

    nlohmann::json data = nlohmann::json::parse(file);
    Map map;
    map.width = data["width"];
    map.height = data["height"];
    map.tileWidth = data["tilewidth"];
    map.tileHeight = data["tileheight"];
    map.tilesets = std::vector<std::tuple<int, std::string>>();

    for (auto& tileset: data["tilesets"])
    {
        std::string source = tileset["source"];
        size_t lastSlash = source.find_last_of("/\\");
        std::string filename = source.substr(lastSlash + 1);
        size_t lastDot = filename.find_last_of(".");
        std::string tilesetID = filename.substr(0, lastDot);

        TilesetManager::loadJSON(tilesetID);
        map.tilesets.push_back({ tileset["firstgid"], tilesetID });
    }

    // Not checking layer name, type and dimensions.
    // Here we assume every layer has same dimensions and must be
    // drawn at given order.
    for (auto& tiledLayer: data["layers"])
    {
        int width = tiledLayer["width"];
        int height = tiledLayer["height"];
        auto data = tiledLayer["data"];
        std::vector<int> mapLayer;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int i = y * width + x;
                int tileID = data[i];
                mapLayer.push_back(tileID);
            }
        }
        map.layers.emplace_back(mapLayer);
    }

    maps[mapID] = map;
    return absl::OkStatus();
}

void MapManager::draw(const std::string mapID)
{
    auto map = maps[mapID];

    for (auto& layer: map.layers)
    {
        int yPos = 0;
        int xPos = 0;
        for (int y = 0; y < map.height; y++)
        {
            xPos = 0;
            yPos += 44;
            for (int x = 0; x < map.width; x++)
            {
                int i = y * map.width + x;
                const unsigned tileGID = layer[i];
                if (tileGID > 0)
                {
                    auto gridPos = Vector2D(xPos, yPos);
                    std::string tilesetID = std::get<1>(map.tilesets[0]);
                    int tileID = map.getTileID(tileGID);
                    auto tileset = TilesetManager::getTileset(tilesetID);
                    auto tile = tileset.getTile(tileID);
                    if (tile.isAnimated())
                    {
                        // std::cout << "Animated tile: " << tileID << std::endl;
                        auto tileIDs = std::get<0>(tile.frames);
                        auto speed = std::get<1>(tile.frames);
                        TileManager::addAnimatedTile(gridPos, tilesetID, tileIDs, speed);
                    }
                    else
                    {
                        auto texture = TilesetManager::getTileTexture(tilesetID, tileID);
                        TileManager::addTile(gridPos, texture);
                    }
                }
                xPos += 44;
            }
        }
    }
}
