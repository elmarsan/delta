// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "MapManager.h"

#include "Asset.h"
#include "Game.h"
#include "Engine.h"
#include "TileManager.h"
#include "Tiled.h"

#include <absl/log/log.h>
#include <absl/status/status.h>
#include <absl/strings/str_format.h>

const unsigned TiledFlippedHorizontally = 0x80000000;
const unsigned TiledFlippedVertically = 0x40000000;

absl::Status DeltaMapManager::draw(const std::string mapId, const Point2 gridPos)
{
    auto map = get(mapId);

    if (map == nullptr)
    {
        return absl::NotFoundError(absl::StrFormat("Map '%s' not found", mapId));
    }

    int tileOffset = 1;
    std::vector<std::string> tilesetIds;
    for (auto& tileset: map->tilesets)
    {
        tileOffset = std::get<0>(tileset);
        tilesetIds.emplace_back(std::get<1>(tileset));
    }

    for (const auto& layer: map->layers)
    {
        int yPos = gridPos.y;
        int xPos = 0;

        for (int y = 0; y < map->height; y++)
        {
            xPos = 0;
            yPos += 44;
            for (int x = 0; x < map->width; x++)
            {
                int i = y * map->width + x;
                const unsigned tileGid = layer[i];
                if (tileGid > 0)
                {
                    int tileID = tileGid;
                    tileID &= ~(TiledFlippedHorizontally | TiledFlippedVertically);

                    auto gridPoint2 = Point2(xPos, yPos);
                    auto tile = Engine::tileset().getTile(tilesetIds, tileGid, tileOffset);
                    Engine::terrain().addCell(gridPoint2, tile);
                }
                xPos += 44;
            }
        }
    }

    return absl::OkStatus();
}

[[nodiscard]] Map* DeltaMapManager::get(const std::string id)
{
    auto it = maps.find(id);
    if (it != maps.end())
    {
        return it->second.get();
    }

    auto loadRes = Tiled::loadJSONMap(id);
    if (!loadRes.ok())
    {
        LOG(ERROR) << loadRes.status().message();
        return nullptr;
    }

    auto tiledMap = loadRes.value();
    auto map = std::make_unique<Map>();
    map->width = tiledMap.width;
    map->height = tiledMap.height;
    map->tileWidth = tiledMap.tileWidth;
    map->tileHeight = tiledMap.tileHeight;

    for (const auto& tiledTileset: tiledMap.tilesets)
    {
        map->tilesets.push_back({ tiledTileset.firstGid, tiledTileset.tilesetID });
    }

    for (const auto& tileLayer: tiledMap.tileLayers)
    {
        map->layers.emplace_back(tileLayer.data);
    }

    for (const auto& objLayer: tiledMap.objectLayers)
    {
        // Parse and scale polygons.
        for (const auto& tiledPolygon: objLayer.polygons)
        {
            Vertex vertex;
            float x = tiledPolygon.x;
            float y = tiledPolygon.y;

            for (const auto& tiledPoint: tiledPolygon.vertex)
            {
                float vx = ((tiledPoint.x + x) / map->tileWidth) * 44;
                float vy = ((tiledPoint.y + y) / map->tileHeight) * 44;
                SDL_FPoint fp { vx, vy };
                vertex.emplace_back(Vec2(vx, vy));
            }

            map->planes.emplace_back(std::make_unique<Polygon>(vertex));
        }

        for (const auto& tiledRect: objLayer.rects)
        {
            float rx = (tiledRect.x / map->tileWidth) * 44;
            float ry = (tiledRect.y / map->tileHeight) * 44;
            float rw = (tiledRect.width / map->tileWidth) * 44;
            float rh = (tiledRect.height / map->tileHeight) * 44;

            map->planes.emplace_back(std::make_unique<Rect>(rx, ry, rw, rh));
        }
    }

    maps[id] = std::move(map);
    return maps[id].get();
}
