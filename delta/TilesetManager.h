#pragma once

#include "Tileset.h"
#include "engine/Vector2D.h"

#include <map>
#include <string>

const unsigned TiledFlippedHorizontally = 0x80000000;
const unsigned TiledFlippedVertically = 0x40000000;

class TilesetManager
{
  private:
    static std::map<std::string, Tileset> tilesets;

  public:
    static void loadJSON(const std::string tilesetID);
    static Tileset getTileset(const std::string tilesetID);
    static Texture getTileTexture(const std::string tilesetID, const unsigned flaggedTileID);
    // static Vector2D getTilePosition(const std::string tilesetID, const unsigned flaggedTileID);
    // static Animation getTileAnimation(const std::string tilesetID, const int tileID);
};
