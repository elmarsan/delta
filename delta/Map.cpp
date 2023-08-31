#include "Map.h"

#include "TilesetManager.h"

int Map::getTileID(int tileGID) const
{
    for (auto& tileset: tilesets)
    {
        auto mapTileset = TilesetManager::getTileset(std::get<1>(tileset));
        int mapTilesetFirstGID = std::get<0>(tileset);

        if (tileGID >= mapTilesetFirstGID && tileGID <= mapTileset.count - 1)
        {
            return tileGID - mapTilesetFirstGID;
        }
    }

    return -1;
}
