#include "Game.h"

#include "Map.h"
#include "engine/Animation.h"
#include "engine/AssetManager.h"
#include "engine/Components.h"
#include "engine/ECS.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <memory>

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
AssetManager* Game::assets = new AssetManager(&manager);

auto& player(manager.addEntity());

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayer));
auto& colliders(manager.getGroup(Game::groupCollider));

Map* map;

Game::~Game()
{
    clean();
}

bool Game::init(int x, int y, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return 0;
    if (IMG_Init(IMG_INIT_PNG) == 0)
        return 0;
    window = SDL_CreateWindow(
        "Delta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    running = true;

    SDL_Color colorMod { 255, 0, 228 };
    assets->addTexture("terrain_water", "data/32water.png");
    assets->addTexture("terrain_grass", "data/32grass.png");
    assets->addTexture("player", "data/p1.png", &colorMod);

    player.addComponent<TransformComponent>(132, 132, 44, 44, 1);
    auto& sprite = player.addComponent<SpriteComponent>(14, 21, "player");
    sprite.addAnimation(
        "walk_up",
        Animation(150, std::vector<Vector2D> { Vector2D(15, 0), Vector2D(15, 22), Vector2D(15, 44) }));
    sprite.addAnimation(
        "walk_down",
        Animation(150, std::vector<Vector2D> { Vector2D(0, 0), Vector2D(0, 22), Vector2D(0, 44) }));
    sprite.addAnimation(
        "walk_lateral",
        Animation(150, std::vector<Vector2D> { Vector2D(30, 0), Vector2D(31, 22), Vector2D(30, 44) }));
    player.addComponent<KeyboardControllerComponent>();
    player.addComponent<ColliderComponent>("Player");
    player.addGroup(Game::groupPlayer);

    map = new Map();
    map->load("data/test_map.json");
    return true;
}

void Game::handleEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event) != 0)
    {
        Game::event = event;

        if (event.type == SDL_QUIT)
        {
            running = false;
        }
    }
}

void Game::update()
{
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    manager.refresh();
    manager.update();

    for (auto& c: colliders)
    {
        ColliderComponent& cCol = c->getComponent<ColliderComponent>();
        if (Collision::AABB(cCol.collider, playerCol))
        {
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto& t: tiles)
    {
        t->draw();
    }
    for (auto& c: colliders)
    {
        c->draw();
    }
    for (auto& p: players)
    {
        p->draw();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

bool Game::isRunning()
{
    return running;
}
