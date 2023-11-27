// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "Tiled.h"

#include "Asset.h"
#include "absl/log/log.h"
#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include "nlohmann/json.hpp"

#include <filesystem>
#include <fstream>

namespace Tiled
{

absl::StatusOr<Map> loadJSONMap(const std::string_view name)
{
    std::string path = absl::StrFormat("data/maps/export/%s.json", name);
    if (!std::filesystem::exists(path))
        return absl::NotFoundError(absl::StrFormat("Map: %s not found", name));

    std::ifstream file(path);
    if (!file.is_open())
        return absl::InternalError("Unable to open the map file");

    LOG(INFO) << absl::StrFormat("Loading Map: %s", name);

    nlohmann::json data = nlohmann::json::parse(file);
    Map map;
    map.renderOrder = data["renderorder"];
    map.orientation = data["orientation"];
    map.tileWidth = data["tilewidth"];
    map.tileHeight = data["tileheight"];
    map.width = data["width"];
    map.height = data["height"];

    for (auto& layer: data["layers"])
    {
        // Tile layer.
        std::string type = layer["type"];
        if (type == "tilelayer")
        {
            DLOG(INFO) << "Parsing tiled tilelayer";

            TileLayer tileLayer;
            tileLayer.name = layer["name"];
            tileLayer.width = layer["width"];
            tileLayer.height = layer["height"];
            std::vector<int> data = layer["data"];
            tileLayer.data = data;

            map.tileLayers.push_back(tileLayer);
        }
        // Object layer.
        else if (type == "objectgroup")
        {
            DLOG(INFO) << "Parsing tiled objectgroup";
            ObjectLayer objectLayer;
            auto objects = layer["objects"];

            objectLayer.ID = layer["id"];
            objectLayer.name = layer["name"];
            objectLayer.draworder = layer["draworder"];

            for (auto& obj: objects)
            {
                int id = obj["x"];
                auto x = obj["x"];
                auto y = obj["y"];
                auto w = obj["width"];
                auto h = obj["height"];
                auto rotation = obj["rotation"];
                auto name = obj["name"];
                auto polygon = obj["polygon"];

                bool isPolygon = !polygon.is_null() && polygon.is_array() && w == 0 && h == 0;
                bool isRect = x >= 0 && y >= 0 && w >= 0 && h >= 0;

                if (isPolygon)
                {
                    Polygon poly;
                    poly.x = x;
                    poly.y = y;
                    poly.rotation = rotation;
                    poly.name = name;

                    for (auto& vertex: polygon)
                    {
                        poly.vertex.push_back(Point { vertex["x"], vertex["y"] });
                    }

                    objectLayer.polygons.push_back(poly);
                }
                else if (isRect)
                {

                    Rect rect;
                    rect.x = x;
                    rect.y = y;
                    rect.width = w;
                    rect.height = h;
                    rect.rotation = rotation;
                    rect.name = name;

                    objectLayer.rects.push_back(rect);
                }
                else
                {
                    LOG(WARNING) << "Unsupported tiled object";
                }
            }

            map.objectLayers.push_back(objectLayer);
        }
    }

    // Tilesets
    for (auto& tileset: data["tilesets"])
    {
        if (tileset.is_object())
        {
            int firstgid = tileset["firstgid"];
            std::string source = tileset["source"];
            size_t lastSlash = source.find_last_of("/\\");
            std::string filename = source.substr(lastSlash + 1);
            size_t lastDot = filename.find_last_of(".");
            std::string tilesetID = filename.substr(0, lastDot);

            MapTileset mapTileset { firstgid, tilesetID };
            map.tilesets.push_back(mapTileset);
        }
    }

    return map;
}

absl::StatusOr<Tileset> loadJSONTileset(const std::string_view name)
{
    std::string path = absl::StrFormat("data/tiles/export/%s.json", name);
    if (!std::filesystem::exists(path))
    {
        return absl::NotFoundError(absl::StrFormat("Tileset: %s not found", name));
    }

    LOG(INFO) << absl::StrFormat("Loading tiled Tileset: %s", name);
    std::ifstream f(path);
    nlohmann::json data = nlohmann::json::parse(f);

    Tileset tileset;
    tileset.name = data["name"];
    tileset.columns = data["columns"];
    tileset.margin = data["margin"];
    tileset.spacing = data["spacing"];
    tileset.tileCount = data["tilecount"];
    tileset.tileWidth = data["tilewidth"];
    tileset.tileHeight = data["tilewidth"];
    tileset.margin = data["margin"];

    // Extract image name from path.
    std::string image = data["image"];
    size_t lastSlash = image.find_last_of("/\\");
    std::string filename = image.substr(lastSlash + 1);
    size_t lastDot = filename.find_last_of(".");
    std::string imageName = filename.substr(0, lastDot);
    tileset.image = imageName;

    // Load animations and colliders.
    for (auto& t: data["tiles"])
    {
        Tile tile;

        // Check if it's animated.
        auto tileAnimation = t["animation"];
        if (!tileAnimation.is_null())
        {
            for (auto& frame: tileAnimation)
            {
                tile.frames.emplace_back(TileFrame {
                    frame["tileid"],
                    frame["duration"],
                });
            }
        }

        // Check if tile has collider.
        auto objectGroup = t["objectgroup"];
        if (!objectGroup.is_null() && objectGroup.is_object())
        {
            auto objects = objectGroup["objects"];
            // Has collider, get it's data from first object.
            if (!objects.is_null() && objects.is_array())
            {
                auto collider = objects[0];
                tile.colliderHeight = collider["height"];
                tile.colliderWidth = collider["width"];
                tile.colliderX = collider["x"];
                tile.colliderY = collider["y"];
            }
        }

        // Extra properties.
        auto properties = t["properties"];
        if (!properties.is_null() && properties.is_array())
        {
            // We only take care of zindex.
            for (auto& p: properties)
            {
                auto name = p["name"];
                if (!name.is_null() && name == "zindex")
                {
                    tile.zindex = p["value"];
                }
            }
        }

        tileset.tiles.push_back(tile);
    }

    return tileset;
}
}; // namespace Tiled
