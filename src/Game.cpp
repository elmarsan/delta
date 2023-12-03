// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "Game.h"

#include "Asset.h"
#include "CharacterController.h"
#include "ColliderComponent.h"
#include "ECS.h"
#include "MapManager.h"
#include "Npc.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "TileComponent.h"
#include "TransformComponent.h"
#include "WindowManager.h"
#include "WorldManager.h"
#include "absl/log/log.h"
#include "absl/status/status.h"
#include "math/Polygon.h"
#include "math/Rect.h"
#include "math/Vec2.h"
#include "sol/types.hpp"
#include "src/BehaviourComponent.h"
#include "src/DetectorComponent.h"
#include "Engine.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <memory>
#include <sol/sol.hpp>
#include <vector>

const int mapWidth = 1024;
const int mapHeight = 1024;

std::shared_ptr<Texture> background;

Manager manager;

std::shared_ptr<Player> player;
std::shared_ptr<Entity> npc3;
Rect rect(396, 396, 44 * 3, 44 * 3);

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayer));
auto& colliders(manager.getGroup(Game::groupCollider));

WorldMap currentMap;

sol::state lua;

absl::Status Game::init()
{
    // Creates lua types
    lua.open_libraries(sol::lib::base);

    sol::usertype<Entity> entityType = lua.new_usertype<Entity>("entity");
    entityType["active"] = &Entity::isActive;

    sol::usertype<Vec2> vec2Type = lua.new_usertype<Vec2>("vec2", sol::constructors<Vec2(float, float)>());
    vec2Type.set("x", sol::readonly(&Vec2::x));
    vec2Type.set("y", sol::readonly(&Vec2::y));

    sol::usertype<TransformComponent> transformComponentType =
        lua.new_usertype<TransformComponent>("transform");
    transformComponentType.set("point2", sol::readonly<Point2>(&TransformComponent::point2));
    transformComponentType.set("size2", sol::readonly<Size2>(&TransformComponent::size2));

    sol::usertype<SDL_FRect> cameraType = lua.new_usertype<SDL_FRect>("camera");
    cameraType.set("x", sol::readonly(&SDL_FRect::x));
    cameraType.set("y", sol::readonly(&SDL_FRect::y));
    cameraType.set("w", sol::readonly(&SDL_FRect::w));
    cameraType.set("h", sol::readonly(&SDL_FRect::h));

    player = manager.addEntity<Player>();
    auto initialMapRes = WorldManager::Instance()->findMapFromPos(player->currentPos());
    LOG_IF(FATAL, !initialMapRes.ok()) << initialMapRes.status().message();
    auto setMapRes = WorldManager::Instance()->setCurrentMap(initialMapRes.value().getID());
    LOG_IF(FATAL, !setMapRes.ok()) << setMapRes.message();

    // Link
    // TODO: Camera binding with new actor system.
    // lua["camera"] = &WindowManager::Instance()->camera;
    lua["player_transform"] = &player->getComponent<TransformComponent>();

    lua.new_enum<Action>("action", { { "go_north", Action::GoNorth }, { "go_south", Action::GoSouth } });
    lua.new_enum<NpcType>("npc_type", { { "fat_man_blue", NpcType::FatManBlue } });
    lua.set_function("add_npc", addNpc);

    addNpc(Point2(440, 308),
           NpcType::FatManBlue,
           Behaviour {
               // Action::Idle,
               Action::RotEast,
               // Action::Idle,
               // Action::RotNorth,
               // Action::Idle,
               Action::RotWest,
               // Action::Idle,
               // Action::RotSouth,
           });

    addNpc(Point2(308, 264),
           NpcType::FatManBlue,
           Behaviour {
               // Action::Idle,
               Action::RotEast,
               // Action::Idle,
               Action::RotNorth,
               // Action::Idle,
               Action::RotWest,
               // Action::Idle,
               Action::RotSouth,
           });

    npc3 = addNpc(Point2(440, 440), NpcType::FatManBlue, Behaviour { Action::Random }, false);
    npc3->getComponent<BehaviourComponent>().plane2 = &rect;

    // lua.script(R"(
    //      npc1 = add_npc(vec2.new(396, 220), npc_type.fat_man_blue, {
    //         action.go_north,
    //         action.go_south,
    //         action.go_south,
    //         action.go_north,
    //         action.go_north,
    //         action.go_south,
    //         action.go_south,
    //      });

    //     npc2 = add_npc(vec2.new(352, 220), npc_type.fat_man_blue, {
    //         action.go_north,
    //         action.go_south,
    //         action.go_south,
    //         action.go_north,
    //         action.go_north,
    //         action.go_south,
    //         action.go_south,
    //      })
    // )");

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
                case SDLK_UP: player->go(Direction::North); break;
                case SDLK_DOWN: player->go(Direction::South); break;
                case SDLK_LEFT: player->go(Direction::West); break;
                case SDLK_RIGHT: player->go(Direction::East); break;
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

    // auto camera = WindowManager::Instance()->camera;
    auto camPos = Engine::actor().getCameraPos();
    auto camSize = Engine::actor().getCameraSize();
    auto windowSize = Engine::window().getSize();

    camPos.x = playerPos.x - windowSize.w / 2;
    camPos.y = playerPos.y - windowSize.h / 2;

    if (camPos.x < 0)
        camPos.x = 0;
    if (camPos.x > mapWidth - camSize.w)
        camPos.x = mapWidth - camSize.w;
    // if (camera.y < 0)
    //     camera.y = 0;
    if (camPos.y > mapHeight - camSize.h)
        camPos.y = mapHeight - camSize.h;

    Engine::actor().setCameraPos(Vec2(camPos.x, camPos.y));
    // WindowManager::Instance()->camera = cameraPos;
}

void Game::render()
{
    Engine::render().clear();
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
    Engine::render().render();
}

bool Game::isRunning()
{
    return running;
}
