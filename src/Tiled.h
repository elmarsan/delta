// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Asset.h"
#include "absl/status/statusor.h"

#include <string>
#include <vector>

namespace Tiled
{
struct TileFrame
{
    float duration;
    int tileID;
};

struct Tile
{
    int ID;
    std::string draworder;
    std::string name;
    int zindex;
    int colliderHeight;
    int colliderWidth;
    int colliderX;
    int colliderY;
    std::vector<TileFrame> frames;

    inline bool hasCollider() const { return colliderHeight > 0 && colliderWidth > 0; }
    inline bool isAnimated() const { return !frames.empty(); }
};

struct Tileset
{
    int columns;
    std::string image;
    std::string name;
    int tileHeight;
    int tileWidth;
    int spacing;
    int tileCount;
    int imageWidth;
    int imageHeight;
    int margin;
    std::string type;
    std::vector<Tile> tiles;
};

struct TileLayer
{
    int id;
    std::vector<int> data;
    std::string name;
    int height;
    int width;
};

struct Rect
{
    int ID;
    float x;
    float y;
    float height;
    float width;
    std::string name;
    float rotation;
};

struct Point
{
    float x;
    float y;
};

struct Polygon
{
    int ID;
    float x;
    float y;
    float height;
    float width;
    std::string name;
    float rotation;
    std::vector<Point> vertex;
};

struct ObjectLayer
{
    int ID;
    std::string name;
    std::string draworder;
    std::vector<Rect> rects;
    std::vector<Polygon> polygons;
};

struct MapTileset 
{
    int firstGid;
    std::string tilesetID;
};

struct Map
{
    std::string renderOrder;
    std::string orientation;
    int tileWidth;
    int tileHeight;
    int height;
    int width;
    std::vector<TileLayer> tileLayers;
    std::vector<ObjectLayer> objectLayers;
    std::vector<MapTileset> tilesets;
};

absl::StatusOr<Map> loadJSONMap(const std::string_view name);
absl::StatusOr<Tileset> loadJSONTileset(const std::string_view name);

}; // namespace Tiled
