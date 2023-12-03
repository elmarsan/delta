// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "TilesetManager.h"

#include "Tiled.h"
#include "absl/status/status.h"

[[nodiscard]] Tileset* TiledTilesetManager::get(const std::string id)
{
    auto it = tilesets.find(id);
    if (it != tilesets.end())
    {
        return it->second.get();
    }

    auto loadRes = Tiled::loadJSONTileset(id);
    if (!loadRes.ok())
    {
        LOG(ERROR) << loadRes.status().message();
        return nullptr;
    }

    auto tiledTileset = loadRes.value();
    Tileset tileset;
    tileset.setID(id);
    tileset.columns = tiledTileset.columns;
    tileset.margin = tiledTileset.margin;
    tileset.numTiles = tiledTileset.tileCount;
    tileset.tileWidth = tiledTileset.tileWidth;
    tileset.tileHeight = tiledTileset.tileHeight;
    tileset.textureID = tiledTileset.image;

    for (const auto& t: tiledTileset.tiles)
    {
        if (t.hasCollider())
        {
            tileset.tileCollider[t.ID] = true;
        }

        if (t.isAnimated())
        {
            std::vector<int> frames;
            int speed;
            for (const auto& f: t.frames)
            {
                frames.push_back(f.tileID);
                speed = f.duration;
            }
        }
    }

    tilesets[id] = std::make_unique<Tileset>(tileset);
    return tilesets[id].get();
}

[[nodiscard]] Tile TiledTilesetManager::getTile(const std::vector<std::string> tilesetIds,
                                                int tileGid,
                                                int offset)
{
    Tile tile;
    for (auto& id: tilesetIds)
    {
        auto tileset = get(id);
        if (tileset == nullptr)
        {
            continue;
        }

        if (tileGid >= offset && tileGid <= tileset->numTiles - 1)
        {
            int tileID = tileGid - offset;
            return tileset->getTile(tileID);
        }
    }

    return tile;
}
