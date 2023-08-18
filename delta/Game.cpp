#include "Game.h"

#include "../engine/Animation.h"
#include "../engine/Components.h"
#include "../engine/ECS.h"
#include "../engine/TextureManager.h"
#include "../engine/Vector2D.h"
#include "Map.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <memory>

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
std::vector<ColliderComponent*> Game::colliders;

Manager manager;
auto& player(manager.addEntity());

Map* map;

enum GroupLabel
{
    MMap,
    Players,
    Colliders,
};

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
    player.addComponent<TransformComponent>(0, 0, 44, 44, 1);
    auto& sprite = player.addComponent<SpriteComponent>(14, 21, "data/p1.png", &colorMod);
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
    player.addGroup(GroupLabel::Players);

    map = new Map();
    map->load();
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
    manager.refresh();
    manager.update();
}

auto& tiles(manager.getGroup(GroupLabel::MMap));
auto& players(manager.getGroup(GroupLabel::Players));
auto& colliders(manager.getGroup(GroupLabel::Colliders));

void Game::render()
{
    SDL_RenderClear(renderer);
    map->draw();
    for (auto& t: tiles)
    {
        t->draw();
    }
    for (auto& p: players)
    {
        p->draw();
    }
    for (auto& c: colliders)
    {
        c->draw();
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
