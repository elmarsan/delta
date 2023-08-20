#include "Map.h"

#include "delta/Game.h"
#include "engine/Components.h"
#include "engine/ECS.h"
#include "engine/Texture.h"
#include "engine/TextureManager.h"
#include "engine/Vector2D.h"
#include "json.hpp"

#include <SDL.h>
#include <fstream>
#include <regex>

extern Manager manager;

TileSetManager tileSetManager;

const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;

Texture getTileTexturePosition(const unsigned rawGID, const Map& map)
{
    bool flipped_horizontally = (rawGID & FLIPPED_HORIZONTALLY_FLAG);
    bool flipped_vertically = (rawGID & FLIPPED_VERTICALLY_FLAG);
    int GID = rawGID;
    GID &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG);

    for (auto& t: map.tilesets)
    {
        auto tileset = tileSetManager.getTileset(t.name);

        if (GID >= t.firstGID && GID <= tileset.count - 1)
        {
            int tileID = GID - t.firstGID;
            int x = (tileID % tileset.columns) * tileset.tileWidth;
            int y = (tileID / tileset.columns) * tileset.tileHeight;

            auto flip = SDL_FLIP_NONE;
            if (flipped_horizontally)
            {
                flip = SDL_FLIP_HORIZONTAL;
            }
            if (flipped_vertically)
            {
                flip = SDL_FLIP_VERTICAL;
            }
            return Texture { tileset.texture, flip, Vector2D(x, y) };
        }
    }

    return Texture {};
}

void Map::load(std::string id)
{
    std::cout << "Loading map: " << id << std::endl;
    std::ifstream f("data/maps/export/" + id + ".json");
    nlohmann::json data = nlohmann::json::parse(f);

    width = data["width"];
    height = data["height"];
    tileWidth = data["tilewidth"];
    tileHeight = data["tileheight"];

    for (auto& tileset: data["tilesets"])
    {
        std::string source = tileset["source"];
        size_t lastSlash = source.find_last_of("/\\");
        std::string filename = source.substr(lastSlash + 1);
        size_t lastDot = filename.find_last_of(".");
        source = filename.substr(0, lastDot);
        tilesets.push_back(MapTileset { tileset["firstgid"], source });
    }

    for (auto& g: data["layers"])
    {
        auto group = std::map<std::string, MapLayer>();
        std::string groupName = g["name"];

        for (auto& l: g["layers"])
        {
            MapLayer layer;

            layer.name = l["name"];
            layer.id = l["id"];
            layer.type = l["type"];

            if (layer.type == "tilelayer")
            {
                layer.width = l["width"];
                layer.height = l["height"];

                for (int y = 0; y < layer.height; y++)
                {
                    for (int x = 0; x < layer.width; x++)
                    {
                        int i = y * layer.width + x;
                        int tileID = l["data"][i];
                        layer.data.push_back(tileID);
                    }
                }
            }
            else if (layer.type == "objectgroup" && layer.name == "transition")
            {
                for (auto object: l["objects"])
                {
                    std::cout << object["properties"] << std::endl;
                    std::string level = object["properties"][0]["value"];
                    if (level == "null") 
                    {
                        level = "";
                    }
                    std::string map = object["properties"][1]["value"];
                    if (map == "null") 
                    {
                        map = "";
                    }
                    int dstX = object["properties"][2]["value"];
                    int dstY = object["properties"][3]["value"];
                    int tileX = object["x"];
                    int tileY = object["y"];
                    int height = object["height"];

                    int srcX = std::floor(tileX / tileWidth);
                    int srcY = std::floor(tileY / tileHeight);
                    Vector2D src(srcX * 44, srcY * 44 + height);
                    Vector2D dst(dstX, dstY);

                    addTeleport(src, dst, map, level);
                }
            }
            group[layer.name] = layer;
        }

        groups[groupName] = group;
    }
}

void Map::draw(std::string level)
{
    auto terrain = groups[level]["terrain_base"];
    auto object = groups[level]["object"];
    auto collision = groups[level]["collision"];

    int yPos = 0;
    int xPos = 0;
    for (int y = 0; y < terrain.height; y++)
    {
        xPos = 0;
        yPos += 44;
        for (int x = 0; x < terrain.width; x++)
        {
            int i = y * terrain.width + x;

            // Collision
            bool addCollider = false;
            int collisionTileID = collision.data[i];
            if (collisionTileID > 0)
            {
                addCollider = true;
            }

            // Terrain
            const unsigned terrainTileID = terrain.data[i];
            if (terrainTileID > 0)
            {
                Texture texture = getTileTexturePosition(terrainTileID, *this);
                Vector2D pos(xPos, yPos);
                addTile(pos, texture, addCollider);
            }

            // Object
            const unsigned objectTileID = object.data[i];
            if (objectTileID > 0)
            {
                Texture texture = getTileTexturePosition(objectTileID, *this);
                Vector2D pos(xPos, yPos);
                addTile(pos, texture, addCollider);
            }
            xPos += 44;
        }
    }
}

void Map::drawRoof()
{
}

void Map::addTile(Vector2D position, Texture texture, bool collider)
{
    auto& tile(manager.addEntity());
    tile.addGroup(Game::groupMap);
    tile.addComponent<TileComponent>(position, texture);

    if (collider)
    {
        tile.addComponent<TransformComponent>(position, 44, 44, 1);
        tile.addComponent<ColliderComponent>("Tile");
        tile.addGroup(Game::groupCollider);
    }
}

void Map::addTeleport(Vector2D source, Vector2D destiny, std::string mapID, std::string level)
{
    std::cout << source << " " << destiny << " " << mapID << std::endl;
    auto& teleport(manager.addEntity());
    teleport.addComponent<TeleportComponent>(source, destiny, mapID, level);
    teleport.addGroup(Game::groupTeleport);
}

void Map::destroyTiles()
{
    for (auto& t: manager.getGroup(Game::groupMap))
    {
        t->deleteGroup(Game::groupMap);
        t->destroy();
    }
    for (auto& t: manager.getGroup(Game::groupTeleport))
    {
        t->deleteGroup(Game::groupTeleport);
        t->destroy();
    }
    for (auto& c: manager.getGroup(Game::groupCollider))
    {
        c->deleteGroup(Game::groupCollider);
        c->destroy();
    }
}
