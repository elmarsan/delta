#include "Game.h"

#include "ColliderComponent.h"
#include "MapManager.h"
#include "SpriteComponent.h"
#include "TileComponent.h"
#include "TileManager.h"
#include "WindowManager.h"
#include "WorldManager.h"
#include "absl/log/log.h"
#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include "engine/Animation.h"
#include "engine/AssetManager.h"
#include "engine/ECS.h"
#include "engine/KeyboardControllerComponent.h"
#include "engine/TransformComponent.h"
#include "engine/Vector2D.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <memory>

const int mapWidth = 1024;
const int mapHeight = 1024;

std::shared_ptr<Texture> background;

Manager manager;

SDL_Event Game::event;
std::unique_ptr<AssetManager> Game::assetManager = std::make_unique<AssetManager>();

auto& player(manager.addEntity());

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayer));
auto& colliders(manager.getGroup(Game::groupCollider));
auto& teleports(manager.getGroup(Game::groupTeleport));

WorldMap currentMap;

absl::Status Game::init()
{
    running = true;

    SDL_Color colorMod { 255, 0, 228 };
    AssetMetadata meta;
    meta = TextureMetadata { &colorMod };
    auto loadTextureRes = Game::assetManager->load<Texture>("p1", &meta);
    LOG_IF(ERROR, !loadTextureRes.ok()) << loadTextureRes.status().message();

    auto playerTransform = player.addComponent<TransformComponent>(Vector2D(444, 400), 44, 44, 1);
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

    auto initialMapRes = WorldManager::Instance()->findMapFromPos(playerTransform.position);
    LOG_IF(FATAL, !initialMapRes.ok()) << initialMapRes.status().message();
    auto setMapRes = WorldManager::Instance()->setCurrentMap(initialMapRes.value().getID());
    LOG_IF(FATAL, !setMapRes.ok()) << setMapRes.message();
    
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

    auto playerMap = WorldManager::Instance()->findMapFromPos(playerPos);
    auto currentMapID = WorldManager::Instance()->getCurrentMapID();
    if (playerMap->getID() != currentMapID)
    {
        std::cout << "\n\n";
        auto setCurrentRes = WorldManager::Instance()->setCurrentMap(playerMap->getID());
        LOG_IF(ERROR, !setCurrentRes.ok()) << setCurrentRes.message();
    }

    manager.refresh();
    manager.update();

    auto camera = WindowManager::Instance()->camera;
    camera.x = playerPos.x - WindowManager::Instance()->width() / 2;
    camera.y = playerPos.y - WindowManager::Instance()->height() / 2;

    if (camera.x < 0)
        camera.x = 0;
    if (camera.x > mapWidth - camera.w)
        camera.x = mapWidth - camera.w;
    // if (camera.y < 0)
    //     camera.y = 0;
    if (camera.y > mapHeight - camera.h)
        camera.y = mapHeight - camera.h;

    WindowManager::Instance()->camera = camera;

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
    SDL_RenderClear(WindowManager::Instance()->renderer);
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
    SDL_RenderPresent(WindowManager::Instance()->renderer);
}

bool Game::isRunning()
{
    return running;
}
