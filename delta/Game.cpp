#include "Game.h"

#include "Map.h"
#include "../engine/KeyboardControllerComponent.h"
#include "../engine/SpriteComponent.h"
#include "../engine/ColliderComponent.h"
#include "../engine/components.h"
#include "../engine/ECS.h"
#include "../engine/TextureManager.h"
#include "../engine/Vector2D.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <memory>

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
// std::vector<engine::component::Collider*> Game::colliders;

Manager manager;
auto& player(manager.addEntity());
// auto& npc(manager.addEntity());

Map* map;

enum GroupLabel
{
    MMap,
    Players,
    Colliders,
};

// auto& tile0(manager.addEntity());
// auto& tile1(manager.addEntity());
// auto& tile2(manager.addEntity());

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
    engine::SpriteFrames playerFrames {
        { engine::Direction::DOWN, std::vector<Vector2D> { Vector2D(0, 0), Vector2D(0, 22), Vector2D(0, 44) } },
        { engine::Direction::UP, std::vector<Vector2D> { Vector2D(15, 0), Vector2D(15, 22), Vector2D(15, 44) } },
        { engine::Direction::RIGHT, std::vector<Vector2D> { Vector2D(30, 0), Vector2D(31, 22), Vector2D(30, 44) } },
        { engine::Direction::LEFT, std::vector<Vector2D> { Vector2D(30, 0), Vector2D(31, 22), Vector2D(30, 44) } },
    };
    auto playerSprite = new engine::Sprite(playerFrames, "data/p1.png", &colorMod);
    playerSprite->flipDirection(engine::Direction::RIGHT, SDL_FLIP_HORIZONTAL);
    player.addComponent<engine::component::Transform>(0, 0, 44, 44, 1);
    player.addComponent<engine::component::Sprite>(playerSprite, 14, 21, true);
    player.addComponent<engine::component::KeyboardController>();
    player.addComponent<engine::component::Collider>("Player");
    player.addGroup(GroupLabel::Players);

    // npc.addComponent<TransformComponent>(80, 80, 14, 21, 3);
    // npc.addComponent<SpriteComponent>("data/p1.png", &modulation);
    // npc.addComponent<ColliderComponent>("npc");

    map = new Map();
    map->load();
    // tile0.addComponent<TileComponent>(200, 200, 32, 32, TileID::WATER);
    // tile0.addComponent<ColliderComponent>("WATER");
    // tile1.addComponent<TileComponent>(250, 250, 32, 32, TileID::GRASS);
    // tile1.addComponent<ColliderComponent>("GRASS");
    // tile2.addComponent<TileComponent>(150, 150, 32, 32, TileID::GRASS);
    // tile2.addComponent<ColliderComponent>("GRASS");
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

    // for (auto collider: colliders)
    // {
    //     if (Collision::AABB(player.getComponent<ColliderComponent>(), *collider))
    //     {
            //     switch (player.getComponent<TransformComponent>().direction)
            //     {
            //         case Direction::UP: {
            //             std::cout << "UP Collision\n";
            //             player.getComponent<TransformComponent>().velocity.y += 1;
            //             break;
            //         }
            //         case Direction::DOWN: {
            //             std::cout << "DOWN Collision\n";
            //             player.getComponent<TransformComponent>().velocity.y -= 1;
            //             break;
            //         }
            //         case Direction::RIGHT: {
            //             std::cout << "RIGHT Collision\n";
            //             player.getComponent<TransformComponent>().velocity.x -= 1;
            //             break;
            //         }
            //         case Direction::LEFT: {
            //             std::cout << "LEFT Collision\n";
            //             player.getComponent<TransformComponent>().velocity.x += 1;
            //             break;
            //         }
            //     }
    //     }
    // }
}

auto& tiles(manager.getGroup(GroupLabel::MMap));
auto& players(manager.getGroup(GroupLabel::Players));
auto& colliders(manager.getGroup(GroupLabel::Colliders));

void Game::render()
{
    SDL_RenderClear(renderer);

    map->draw();
    // manager.draw();
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

// HERE ADD T0O GROUP MAP
// void Game::addTile(int id, int x, int y)
// {
//     auto& tile(manager.addEntity());

//     auto tileID = TileID::GRASS;
//     if (id == 2) {
//         tileID = TileID::WATER;
//     }

//     tile.addComponent<TileComponent>(x, y, 32, 32, tileID);
// }
