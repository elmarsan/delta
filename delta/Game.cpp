#include "Game.h"

#include "ColliderComponent.h"
#include "SpriteComponent.h"
#include "MapManager.h"
#include "TileComponent.h"
#include "absl/status/status.h"
#include "engine/Animation.h"
#include "engine/CameraComponent.h"
#include "engine/AssetManager.h"
#include "engine/ECS.h"
#include "engine/TeleportComponent.h"
#include "engine/TileManager.h"
#include "engine/TransformComponent.h"
#include "engine/KeyboardControllerComponent.h"
#include "engine/Vector2D.h"

#include "absl/log/log.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <iostream>
#include <memory>

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
std::unique_ptr<AssetManager> Game::assetManager = std::make_unique<AssetManager>();

auto& player(manager.addEntity());

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayer));
auto& colliders(manager.getGroup(Game::groupCollider));
auto& teleports(manager.getGroup(Game::groupTeleport));

auto& camera(manager.addEntity());

Game::~Game()
{
    clean();
}

absl::Status Game::init(int x, int y, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return absl::InternalError("Unable to init SDL Video");
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        return absl::InternalError("Unable to init SDL Image ");
    }

    window = SDL_CreateWindow(
        "Delta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    running = true;

    SDL_Color colorMod { 255, 0, 228 };
    AssetMetadata meta;
    meta = TextureMetadata{ &colorMod};
    auto loadTextureRes = Game::assetManager->load<Texture>("p1", &meta);
    LOG_IF(ERROR, !loadTextureRes.ok()) << loadTextureRes.status().message();

    player.addComponent<TransformComponent>(Vector2D(500, 650), 44, 44, 1);
    auto& sprite = player.addComponent<SpriteComponent>(14, 21, "p1");
    sprite.addAnimation(
        "walk_up",
        new Animation(150, std::vector<Vector2D> { Vector2D(15, 0), Vector2D(15, 22), Vector2D(15, 44) }));
    sprite.addAnimation(
        "walk_down",
        new Animation(150, std::vector<Vector2D> { Vector2D(0, 0), Vector2D(0, 22), Vector2D(0, 44) }));
    sprite.addAnimation(
        "walk_lateral",
        new Animation(150, std::vector<Vector2D> { Vector2D(30, 0), Vector2D(31, 22), Vector2D(30, 44) }));
    player.addComponent<KeyboardControllerComponent>();
    player.addComponent<ColliderComponent>("Player");
    player.addGroup(Game::groupPlayer);

    camera.addComponent<CameraComponent>(5, 5);

    auto loadMapRes = Game::assetManager->load<Map>("littleroot-town");
    LOG_IF(FATAL, !loadTextureRes.ok()) << loadTextureRes.status().message();
    
    auto drawMapRes = MapManager::draw("littleroot-town");
    LOG_IF(FATAL, !loadTextureRes.ok()) << loadTextureRes.status().message();

    return absl::OkStatus();
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

    camera.getComponent<CameraComponent>().setPos(playerPos);

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

    // for (auto& t: teleports)
    // {
    //     auto teleport = t->getComponent<TeleportComponent>();
    //     auto destiny = t->getComponent<TeleportComponent>().destiny;
    //     auto mapID = t->getComponent<TeleportComponent>().mapID;

    //     if (Collision::AABB(teleport.rect, playerCol))
    //     {
    //         if (teleport.level != "")
    //         {
    //             std::cout << "Teleport to level: " << teleport.level << destiny << std::endl;
    //             map->destroyTiles();
    //             map->draw(teleport.level);
    //             player.getComponent<TransformComponent>().position.x = destiny.x * 44;
    //             player.getComponent<TransformComponent>().position.y = destiny.y * 44;
    //         }
    //         else if (teleport.mapID != "")
    //         {
    //             std::cout << "Teleport to map: " << teleport.mapID << destiny << std::endl;
    //             map->destroyTiles();
    //             delete map;
    //             map = new Map();
    //             map->load(teleport.mapID);
    //             map->draw("level_0");
    //             player.getComponent<TransformComponent>().position.x = destiny.x * 44;
    //             player.getComponent<TransformComponent>().position.y = destiny.y * 44;
    //         }
    //     }
    // }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto& t: tiles)
    {
        if (t->getComponent<TileComponent>().zindex() == 0)
        {
            t->draw();
        }
    }
    for (auto& c: colliders)
    {
        c->draw();
    }
    for (auto& p: players)
    {
        p->draw();
    }
    for (auto& t: tiles)
    {
        if (t->getComponent<TileComponent>().zindex() > 0)
        {
            t->draw();
        }
    }
    // for (auto& t: teleports)
    // {
    //     t->draw();
    // }
    camera.draw();
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
