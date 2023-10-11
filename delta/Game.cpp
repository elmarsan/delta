#include "Game.h"

#include "BehaviourComponent.h"
#include "CharacterController.h"
#include "ColliderComponent.h"
#include "DetectorComponent.h"
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
#include "engine/TransformComponent.h"
#include "engine/Vector2.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <memory>

const int mapWidth = 1024;
const int mapHeight = 1024;

std::shared_ptr<Texture> background;

Manager manager;

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

    auto playerTransform = player.addComponent<TransformComponent>(Point2(440, 396), Size2(44, 44));
    auto& sprite = player.addComponent<SpriteComponent>(14, 21, "p1");
    sprite.addAnimation(
        "walk_up", new Animation(150, std::vector<Point2> { Point2(15, 0), Point2(15, 22), Point2(15, 44) }));
    sprite.addAnimation(
        "walk_down", new Animation(150, std::vector<Point2> { Point2(0, 0), Point2(0, 22), Point2(0, 44) }));
    sprite.addAnimation(
        "walk_lateral",
        new Animation(150, std::vector<Point2> { Point2(30, 0), Point2(31, 22), Point2(30, 44) }));
    player.addComponent<CharacterController>();
    player.addComponent<ColliderComponent>("Player");
    player.addGroup(Game::groupPlayer);
    auto initialMapRes = WorldManager::Instance()->findMapFromPos(playerTransform.point2);
    LOG_IF(FATAL, !initialMapRes.ok()) << initialMapRes.status().message();
    auto setMapRes = WorldManager::Instance()->setCurrentMap(initialMapRes.value().getID());
    LOG_IF(FATAL, !setMapRes.ok()) << setMapRes.message();

    auto& npcEntity(manager.addEntity());
    npcEntity.addComponent<TransformComponent>(Point2(440, 308), Size2(44, 44));
    auto& npcSprite = npcEntity.addComponent<SpriteComponent>(14, 21, "p1");
    npcSprite.addAnimation(
        "walk_up", new Animation(150, std::vector<Point2> { Point2(15, 0), Point2(15, 22), Point2(15, 44) }));
    npcSprite.addAnimation(
        "walk_down", new Animation(150, std::vector<Point2> { Point2(0, 0), Point2(0, 22), Point2(0, 44) }));
    npcSprite.addAnimation(
        "walk_lateral",
        new Animation(150, std::vector<Point2> { Point2(30, 0), Point2(31, 22), Point2(30, 44) }));

    npcEntity.addComponent<ColliderComponent>("tile");
    npcEntity.addComponent<CharacterController>();
    // npcEntity.addComponent<BehaviourComponent>();
    npcEntity.addComponent<DetectorComponent>(Size2(0, 44 * 3));
    npcEntity.addGroup(Game::groupCollider);
    npcEntity.addGroup(Game::groupNpc);

    return absl::OkStatus();
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            LOG(INFO) << "SDL_QUIT event";
            LOG(INFO) << "Stopping delta...";
            running = false;
        }

        if (event.type == SDL_KEYDOWN)
        {
            auto& playerController = player.getComponent<CharacterController>();
            auto key = event.key.keysym.sym;
            switch (key)
            {
                case SDLK_UP: {
                    playerController.goNorth();
                    break;
                }
                case SDLK_DOWN: {
                    playerController.goSouth();
                    break;
                }
                case SDLK_LEFT: {
                    playerController.goWest();
                    break;
                }
                case SDLK_RIGHT: {
                    playerController.goEast();
                    break;
                }
            }
        }
    }
}

void Game::update()
{
    // SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    auto playerPoint2 = player.getComponent<TransformComponent>().point2;

    auto playerMap = WorldManager::Instance()->findMapFromPos(playerPoint2);
    auto currentMapID = WorldManager::Instance()->getCurrentMapID();
    if (playerMap->getID() != currentMapID)
    {
        auto setCurrentRes = WorldManager::Instance()->setCurrentMap(playerMap->getID());
        LOG_IF(ERROR, !setCurrentRes.ok()) << setCurrentRes.message();
    }

    manager.refresh();
    manager.update();

    auto camera = WindowManager::Instance()->camera;
    camera.x = playerPoint2.x - WindowManager::Instance()->width() / 2;
    camera.y = playerPoint2.y - WindowManager::Instance()->height() / 2;

    if (camera.x < 0)
        camera.x = 0;
    if (camera.x > mapWidth - camera.w)
        camera.x = mapWidth - camera.w;
    // if (camera.y < 0)
    //     camera.y = 0;
    if (camera.y > mapHeight - camera.h)
        camera.y = mapHeight - camera.h;

    WindowManager::Instance()->camera = camera;

    // for (auto& c: colliders)
    // {
    //     ColliderComponent& cCol = c->getComponent<ColliderComponent>();
    //     if (Collision::AABB(cCol.collider, playerCol))
    //     {
    //     }
    // }
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
        // if (c->hasGroup(Game::groupCollider))
        // {
        //     auto& transform = c->getComponent<TransformComponent>();
        //     if (transform.point2 == Point2(308, 396))
        //         LOG(INFO) << "Find collision in point:" << transform.point2;
        // }
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
