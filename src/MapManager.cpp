#include "MapManager.h"

#include "Asset.h"
#include "Game.h"
#include "TileManager.h"
#include "nlohmann/json.hpp"

#include <absl/log/log.h>
#include <absl/status/status.h>
#include <absl/strings/str_format.h>
#include <filesystem>
#include <fstream>
#include <memory>

const unsigned TiledFlippedHorizontally = 0x80000000;
const unsigned TiledFlippedVertically = 0x40000000;

Tile MapManager::getTileV2(std::vector<std::shared_ptr<Tileset>> tilesets,
                           int tileGID,
                           int mapTilesetFirstGID)
{
    Tile tile;
    for (auto& mapTileset: tilesets)
    {
        if (tileGID >= mapTilesetFirstGID && tileGID <= mapTileset->numTiles - 1)
        {
            int tileID = tileGID - mapTilesetFirstGID;
            return mapTileset->getTile(tileID);
        }
    }

    return tile;
}

absl::Status MapManager::draw(const std::string mapID, const Vector2 gridPos)
{
    LOG(INFO) << absl::StrFormat("Drawing map: %s", mapID);
    auto mapRes = Game::assetManager->getOrLoad<Map>(mapID);
    if (!mapRes.ok())
        return mapRes.status();

    auto map = mapRes.value();

    // Load tilesets
    std::vector<std::shared_ptr<Tileset>> tilesets;
    for (auto& tileset: map->tilesets)
    {
        int mapTilesetFirstGID = std::get<0>(tileset);
        std::string tilesetID = std::get<1>(tileset);

        auto loadRes = Game::assetManager->getOrLoad<Tileset>(tilesetID);
        if (!loadRes.ok())
            LOG(ERROR) << loadRes.status().message();
        else
            tilesets.push_back(loadRes.value());
    }

    for (auto& layer: map->layers)
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
                const unsigned tileGID = layer[i];
                if (tileGID > 0)
                {
                    // bool flippedHorizontally = (tileGID & TiledFlippedHorizontally);
                    // bool flippedVertically = (tileGID & TiledFlippedVertically);
                    // auto flip = SDL_FLIP_NONE;
                    // if (flippedHorizontally)
                    //     flip = SDL_FLIP_HORIZONTAL;
                    // else if (flippedVertically)
                    //     flip = SDL_FLIP_VERTICAL;
                    int tileID = tileGID;
                    tileID &= ~(TiledFlippedHorizontally | TiledFlippedVertically);

                    auto gridPoint2 = Point2(xPos, yPos);
                    auto tile = getTileV2(tilesets, tileID, 1);
                    TileManager::addTile(gridPoint2, tile);
                }
                xPos += 44;
            }
        }
    }

    return absl::OkStatus();
}
