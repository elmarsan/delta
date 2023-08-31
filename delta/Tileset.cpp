#include "Tileset.h"

Tile Tileset::getTile(const int tileID)
{
    int x = (tileID % columns) * (tileWidth + margin) + margin;
    int y = (tileID / columns) * (tileHeight + margin) + margin;

    Tile tile;
    tile.texture = texture;
    tile.tilesetID = ID;
    tile.frames = tileFrames[tileID];
    tile.pos = Vector2D(x, y);
    return tile;
}

bool Tile::isAnimated()
{
    return std::get<0>(frames).size() > 0;
}
