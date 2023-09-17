#pragma once

#include "Vector2D.h"

#include <SDL2/SDL_render.h>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

using AssetID = std::string;

class Asset
{
  public:
    virtual AssetID getID() = 0;
};

class Texture: public Asset
{
  public:
    SDL_Texture* sdlTexture;
    SDL_RendererFlip flip;

    Texture(AssetID ID, SDL_Texture* sdlTexture): sdlTexture(sdlTexture), ID(ID) {}

    AssetID getID() override { return ID; }

  private:
    AssetID ID;
};

struct TextureMetadata
{
    SDL_Color* colorMod;
};

class Map: public Asset
{
  public:
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    std::vector<std::vector<int>> layers;
    std::vector<std::tuple<int, std::string>> tilesets;

    AssetID getID() override { return ID; }

  private:
    AssetID ID;
};

using TileID = int;

using TileFrames = std::tuple<std::vector<int>, int>;
enum TileFrame
{
    FrameIDs,
    Speed
};

using TilesetID = std::string;

struct Tile
{
    TileID ID;
    AssetID textureID;
    AssetID tilesetID;
    Vector2D pos;
    int width;
    int height;
    TileFrames frames;
    bool collides;
    bool isAnimated() { return std::get<TileFrame::FrameIDs>(frames).size() > 0; }

    int numFrames() { return std::get<TileFrame::FrameIDs>(frames).size(); }

    int animSpeed() { return std::get<TileFrame::Speed>(frames); }
};

class Tileset: public Asset
{
  public:
    Tileset() = default;
    Tileset(const AssetID assetID): ID(assetID) {}

    // std::shared_ptr<Texture> texture;
    AssetID textureID;
    std::string image;
    std::map<TileID, TileFrames> tileFrame;
    std::map<TileID, bool> tileCollider;
    int columns;
    int tileWidth;
    int tileHeight;
    int numTiles;
    int imageWidth;
    int imageHeight;
    int margin;
    int spacing;

    Tile getTile(const TileID tileID)
    {
        int x = (tileID % columns) * (tileWidth + margin) + margin;
        int y = (tileID / columns) * (tileHeight + margin) + margin;

        Tile tile;
        tile.ID = tileID;
        tile.textureID = textureID;
        tile.tilesetID = ID;
        tile.frames = tileFrame[tileID];
        tile.pos = Vector2D(x, y);
        tile.width = tileWidth;
        tile.height = tileHeight;
        tile.collides = tileCollider[tileID];
        return tile;
    }

    void setID(AssetID assetID) { ID = assetID; }
    AssetID getID() override { return ID; }

  private:
    AssetID ID;
};

using AssetMetadata = std::variant<TextureMetadata>;
