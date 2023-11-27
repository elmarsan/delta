// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "AssetLoader.h"

#include "Asset.h"
#include "Tiled.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "math/Polygon.h"

#include <SDL2/SDL_image.h>
#include <filesystem>
#include <memory>
#include <vector>

AssetLoadResult TextureLoader::load(const std::string& assetID, AssetMetadata* metadata)
{
    LOG(INFO) << absl::StrFormat("Loading Texture: %s", assetID);
    std::string path = absl::StrFormat("data/assets/%s.png", assetID);
    if (!std::filesystem::exists(path))
    {
        return absl::NotFoundError(absl::StrFormat("Texture: %s not found", assetID));
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (metadata != nullptr)
    {
        SDL_Color* colorMod = std::get<TextureMetadata>(*metadata).colorMod;
        if (colorMod != nullptr)
        {
            auto modulation = SDL_MapRGB(surface->format, colorMod->r, colorMod->g, colorMod->b);
            SDL_SetColorKey(surface, SDL_TRUE, modulation);
        }
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(WindowManager::Instance()->renderer, surface);
    SDL_FreeSurface(surface);
    return std::make_shared<Texture>(assetID, texture);
}

AssetLoadResult TilesetLoader::load(const std::string& assetID, AssetMetadata* metadata)
{
    auto loadRes = Tiled::loadJSONTileset(assetID);
    if (!loadRes.ok())
    {
        return loadRes.status();
    }

    auto tiledTileset = loadRes.value();
    Tileset tileset;
    tileset.setID(assetID);
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

    return std::make_shared<Tileset>(tileset);
}

AssetLoadResult MapLoader::load(const std::string& assetID, AssetMetadata* metadata)
{
    auto loadRes = Tiled::loadJSONMap(assetID);
    if (!loadRes.ok())
    {
        return loadRes.status();
    }
    auto tiledMap = loadRes.value();
    auto map = std::make_shared<Map>();
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

            map->planes.emplace_back(
                std::make_unique<Rect>(rx, ry, rw, rh));
        }
    }

    return map;
}
