#include "Map.h"

#include "../engine/TextureManager.h"
#include "Game.h"
#include "SDL_render.h"

#include <iostream>

int testMap[20][25] = { { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
                        { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 } };
Map::Map()
{
    grassTexture = TextureManager::load("data/32grass.png");
    waterTexture = TextureManager::load("data/32water.png");

    src.x = src.y = 0;
    dst.x = dst.y = 0;
    src.w = dst.w = 32;
    src.h = dst.h = 32;
}

Map::~Map()
{
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(waterTexture);
}

void Map::load()
{
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 25; col++)
        {
            map[row][col] = testMap[row][col];
        }
    }
}

void Map::draw()
{
    SDL_SetRenderDrawColor(Game::renderer, 0xff, 0, 0, 0);
    int type = 0;
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 25; col++)
        {
            type = map[row][col];
            dst.w = dst.h = 44;
            dst.x = col * 44;
            dst.y = row * 44;

            switch (type)
            {
                case 0: {
                    TextureManager::draw(grassTexture, &src, &dst, SDL_FLIP_NONE);
                    break;
                }
                case 2: {
                    TextureManager::draw(waterTexture, &src, &dst, SDL_FLIP_NONE);
                    break;
                }
            }
            SDL_RenderDrawRect(Game::renderer, &dst);
        }
    }
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
}
