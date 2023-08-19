#include "Map.h"

#include "Game.h"
#include "engine/Components.h"
#include "engine/ECS.h"
#include "engine/TextureManager.h"
#include "json.hpp"

#include <SDL.h>
#include <fstream>

extern Manager manager;

Map::Map()
{
    waterTexture = TextureManager::load("data/32water.png");
    grassTexture = TextureManager::load("data/32grass.png");
}

Map::~Map()
{
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(waterTexture);
}

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
    SDL_Texture* texture;
    std::string tag;

    switch (tileId)
    {
        case 1:
            texture = waterTexture;
            tag = "terrain_water";
            break;
        case 2:
            texture = grassTexture;
            tag = "terrain_grass";
            break;
    }

    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, texture);
    tile.addGroup(Game::groupMap);

    if (hasCollider)
    {
        tile.addComponent<TransformComponent>(x, y, 44, 44, 1);
        tile.addComponent<ColliderComponent>("Tile");
        tile.addGroup(Game::groupCollider);
    }
}
