#include "engine/Texture.h"
#include "engine/TextureManager.h"
#include "engine/TileComponent.h"
#include "engine/Vector2D.h"
#include "json.hpp"

#include <SDL.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>

struct MapLayer
{
    std::string name;
    int id;
    int width, height;
    std::vector<int> data;
    std::string type;
};

struct Tileset
{
    std::string name;
    int columns;
    int tileWidth, tileHeight;
    int count;
    int imageWidth, imageHeight;
    int margin;
    int spacing;
    std::string image;
    SDL_Texture* texture;
};

struct MapTileset
{
    int firstGID;
    std::string name;
};

class Map
{
  public:
    void load(std::string id);
    void draw(std::string level);
    void drawRoof();

    void addTile(Vector2D position, Texture texture, bool collider);
    void addTeleport(Vector2D source, Vector2D destiny, std::string mapID = "", std::string level = "");

    void destroyTiles();
    std::vector<MapTileset> tilesets;

  private:
    int width, height;
    int tileWidth, tileHeight;
    std::map<std::string, std::map<std::string, MapLayer>> groups;
};

class TileSetManager
{
  public:
    std::map<std::string, Tileset> tilesets;

    Tileset getTileset(const std::string id)
    {
        if (tilesets.count(id) == 0)
        {
            std::ifstream f("data/tiles/export/" + id + ".json");
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

            size_t lastSlash = tileset.image.find_last_of("/\\");
            std::string filename = tileset.image.substr(lastSlash + 1);
            size_t lastDot = filename.find_last_of(".");
            tileset.image = filename.substr(0, lastDot);
            tileset.texture = TextureManager::load("data/assets/" + tileset.image + ".png");
            tilesets[id] = tileset;

            if (tileset.texture == nullptr)
            {
                std::cout << tileset.image << " null texture" << std::endl;
            }
        }
        return tilesets[id];
    }
};
