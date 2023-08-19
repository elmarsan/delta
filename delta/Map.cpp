#include "Map.h"

#include "Game.h"
#include "engine/Components.h"
#include "engine/ECS.h"
#include "engine/TextureManager.h"
#include "json.hpp"

#include <SDL.h>
#include <fstream>

extern Manager manager;

void Map::load(std::string path)
{
    std::ifstream f(path);
    nlohmann::json data = nlohmann::json::parse(f);

    w = data["width"];
    h = data["height"];
    tileWidth = data["tileWidth"];
    tileHeight = data["tileHeight"];
    auto layers = data["layers"];

    int yPos = 0;
    int xPos = 0;
    for (int y = 0; y < h; y++)
    {
        xPos = 0;
        yPos += 44;
        for (int x = 0; x < w; x++)
        {
            int i = y * w + x;
            int tileID = layers["terrain"][i];
            bool hasCollider = layers["collision"][i] != 0;
            addTile(xPos, yPos, tileID, hasCollider);
            xPos += 44;
        }
    }
}

void Map::addTile(int x, int y, int tileId, bool hasCollider)
{
    auto& tile(manager.addEntity());
    tile.addGroup(Game::groupMap);

    switch (tileId)
    {
        case 1:
            tile.addComponent<TileComponent>(x, y, Game::assets->getTexture("terrain_water"));
            break;
        case 2:
            tile.addComponent<TileComponent>(x, y, Game::assets->getTexture("terrain_grass"));
            break;
    }

    if (hasCollider)
    {
        tile.addComponent<TransformComponent>(x, y, 44, 44, 1);
        tile.addComponent<ColliderComponent>("Tile");
        tile.addGroup(Game::groupCollider);
    }
}
