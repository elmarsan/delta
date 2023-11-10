#include "Game.h"

#include "AssetManager.h"
#include "CharacterController.h"
#include "ColliderComponent.h"
#include "ECS.h"
#include "MapManager.h"
#include "Npc.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "TileComponent.h"
#include "TransformComponent.h"
#include "Vector2.h"
#include "WindowManager.h"
#include "WorldManager.h"
#include "absl/log/log.h"
#include "absl/status/status.h"
#include "sol/types.hpp"
#include "src/RoutineComponent.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <memory>
#include <sol/sol.hpp>

const int mapWidth = 1024;
const int mapHeight = 1024;

std::shared_ptr<Texture> background;

Manager manager;

std::unique_ptr<AssetManager> Game::assetManager = std::make_unique<AssetManager>();

std::shared_ptr<Player> player;

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayer));
auto& colliders(manager.getGroup(Game::groupCollider));
auto& teleports(manager.getGroup(Game::groupTeleport));

WorldMap currentMap;

sol::state lua;

absl::Status Game::init()
{
    // Creates lua types
    lua.open_libraries(sol::lib::base);

    sol::usertype<Entity> entityType = lua.new_usertype<Entity>("entity");
    entityType["active"] = &Entity::isActive;

    sol::usertype<Vector2> vec2Type =
        lua.new_usertype<Vector2>("vec2", sol::constructors<Vector2(int, int)>());
    vec2Type.set("x", sol::readonly(&Vector2::x));
    vec2Type.set("y", sol::readonly(&Vector2::y));

    sol::usertype<TransformComponent> transformComponentType =
        lua.new_usertype<TransformComponent>("transform");
    transformComponentType.set("point2", sol::readonly<Point2>(&TransformComponent::point2));
    transformComponentType.set("size2", sol::readonly<Size2>(&TransformComponent::size2));

    sol::usertype<SDL_Rect> cameraType = lua.new_usertype<SDL_Rect>("camera");
    cameraType.set("x", sol::readonly(&SDL_Rect::x));
    cameraType.set("y", sol::readonly(&SDL_Rect::y));
    cameraType.set("w", sol::readonly(&SDL_Rect::w));
    cameraType.set("h", sol::readonly(&SDL_Rect::h));

    player = manager.addEntity<Player>();
    auto initialMapRes = WorldManager::Instance()->findMapFromPos(player->currentPos());
    LOG_IF(FATAL, !initialMapRes.ok()) << initialMapRes.status().message();
    auto setMapRes = WorldManager::Instance()->setCurrentMap(initialMapRes.value().getID());
    LOG_IF(FATAL, !setMapRes.ok()) << setMapRes.message();

    // Link
    lua["camera"] = &WindowManager::Instance()->camera;
    lua["player_transform"] = &player->getComponent<TransformComponent>();

    lua.new_usertype<GoNorthAction>("GoNorthAction", sol::constructors<GoNorthAction()>());
    lua.new_usertype<GoSouthAction>("GoSouthAction", sol::constructors<GoSouthAction()>());
    lua.set_function("action_go_north",
                     []() -> std::shared_ptr<Action> { return std::make_shared<GoNorthAction>(); });
    lua.set_function("action_go_south",
                     []() -> std::shared_ptr<Action> { return std::make_shared<GoSouthAction>(); });

    lua.set_function("add_npc", AddNpc);
    lua.script(R"(
         v = vec2.new(440, 220)
         actions = {action_go_north, action_go_south}
         npc = add_npc(v, "npc", actions)
    )");
    if (lua["v"].valid())
    {
        LOG(INFO) << "Vector2: " << lua["v"].get<Vector2>();
    }
    // AddNpc(Point2(440, 220), "npc");

    running = true;
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
            auto key = event.key.keysym.sym;
            switch (key)
            {
                case SDLK_UP: player->goNorth(); break;
                case SDLK_DOWN: player->goSouth(); break;
                case SDLK_LEFT: player->goWest(); break;
                case SDLK_RIGHT: player->goEast(); break;
            }
        }
    }
}

void Game::update()
{
    auto playerPos = player->currentPos();
    auto map = WorldManager::Instance()->findMapFromPos(playerPos);
    auto currentMapID = WorldManager::Instance()->getCurrentMapID();
    if (map->getID() != currentMapID)
    {
        auto setCurrentRes = WorldManager::Instance()->setCurrentMap(map->getID());
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

    // lua.script("cam = camera");
    // if (lua["cam"].valid()) {
    //     LOG(INFO) << "X camera " << lua["cam"].get<SDL_Rect>().x;
    //     LOG(INFO) << "Y camera " << lua["cam"].get<SDL_Rect>().y;
    //     LOG(INFO) << "W camera " << lua["cam"].get<SDL_Rect>().w;
    // }

    // lua.script("pt = player_transform.point2");
    // lua.script("psize = player_transform.size2");
    // if (lua["pt"].valid()) {
    //     LOG(INFO) << "Player point2: " << lua["pt"].get<Point2>();
    // }
    // if (lua["psize"].valid()) {
    //     LOG(INFO) << "Player size2: " << lua["psize"].get<Size2>();
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
