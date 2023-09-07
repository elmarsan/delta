#include "TilesetManager.h"

#include "engine/Animation.h"
#include "engine/TextureManager.h"
#include "json.hpp"

#include <fstream>

std::map<std::string, Tileset> TilesetManager::tilesets = std::map<std::string, Tileset>();

Tileset TilesetManager::getTileset(const std::string tilesetID)
{
    if (tilesets.count(tilesetID) == 0)
    {
        loadJSON(tilesetID);
    }
    return tilesets[tilesetID];
}

void TilesetManager::loadJSON(const std::string tilesetID)
{
    if (tilesets.count(tilesetID) == 1)
    {
        return;
    }

    std::ifstream f("data/tiles/export/" + tilesetID + ".json");
    nlohmann::json data = nlohmann::json::parse(f);

    Tileset tileset;
    tileset.columns = data["columns"];
    tileset.image = data["image"];
    tileset.imageWidth = data["imagewidth"];
    tileset.imageHeight = data["imageheight"];
    tileset.margin = data["margin"];
    tileset.spacing = data["spacing"];
    tileset.count = data["tilecount"];
    tileset.tileWidth = data["tilewidth"];
    tileset.tileHeight = data["tilewidth"];

    // Load texture png
    size_t lastSlash = tileset.image.find_last_of("/\\");
    std::string filename = tileset.image.substr(lastSlash + 1);
    size_t lastDot = filename.find_last_of(".");
    tileset.image = filename.substr(0, lastDot);
    tileset.texture = TextureManager::load("data/assets/" + tileset.image + ".png");

    // Load animated tiles
    for (auto& tile: data["tiles"])
    {
        auto tileAnimation = tile["animation"];
        if (!tileAnimation.is_null())
        {
    
            auto frames = std::vector<int>();
            int speed;
            for (auto& frame: tileAnimation)
            {
                frames.emplace_back(frame["tileid"]);
                speed = frame["duration"];
            }

            tileset.tileFrames[tile["id"]] = std::make_tuple(frames, speed);
        }
    }
  
    std::cout << "***********************************" << std::endl;
    std::cout << "Animated tiles" << std::endl;
    for (auto& [k, v]: tileset.tileFrames)
    {
        auto frames = std::get<0>(v);
        auto speed = std::get<1>(v);

        std::cout << "Speed: " <<  speed << std::endl;
        std::cout << "Tile " << k << ": ";
        for (auto& f: frames)
        {
            std::cout << f << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "***********************************" << std::endl;
    std::cout << "\n";

    tilesets[tilesetID] = tileset;
}

Texture TilesetManager::getTileTexture(const std::string tilesetID, const unsigned flaggedTileID)
{
    Tileset tileset = getTileset(tilesetID);
    bool flippedHorizontally = (flaggedTileID & TiledFlippedHorizontally);
    bool flippedVertically = (flaggedTileID & TiledFlippedVertically);

    auto flip = SDL_FLIP_NONE;
    if (flippedHorizontally)
        flip = SDL_FLIP_HORIZONTAL;
    else if (flippedVertically)
        flip = SDL_FLIP_VERTICAL;

    int tileID = flaggedTileID;
    tileID &= ~(TiledFlippedHorizontally | TiledFlippedVertically);

    return Texture { tileset.texture, flip, tileset.getTile(tileID).pos };
}
