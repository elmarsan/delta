#include "Game.h"

#include "Asset.h"
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

std::unique_ptr<AssetManager> Game::assetManager = std::make_unique<AssetManager>();

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
    lua["camera"] = &WindowManager::Instance()->camera;
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
}

void drawPoly(std::vector<SDL_FPoint> points, std::string color)
{
    if (color == "green")
    {
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 255, 0, 255);
    }
    else if (color == "red")
    {
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 255, 0, 0, 255);
    }
    else if (color == "purple")
    {
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 159, 90, 253, 255);
    }
    else if (color == "blue")
    {
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 255, 255);
    }
    SDL_RenderDrawLinesF(WindowManager::Instance()->renderer, points.data(), points.size());
    SDL_RenderDrawLine(WindowManager::Instance()->renderer,
                       points.back().x,
                       points.back().y,
                       points.front().x,
                       points.front().y);
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
    // float x = (244.97 / 16) * 44;
    // float y = (146.675 / 16) * 44;
    // SDL_FRect rect { x, y, 44, 44 };
    // SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0xff, 0, 0, 0);
    // SDL_RenderDrawRectF(WindowManager::Instance()->renderer, &rect);
    // SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 0, 0);

    // ***********************************
    float x = 120.437;
    float y = 30.4235;
    
    std::vector<SDL_FPoint> yetPoints { SDL_FPoint{ 47.789, 7.32649 },
                                        SDL_FPoint{ 0.229755, 121.076 },
                                        SDL_FPoint{ 143.98, 63.033 } };
    std::vector<SDL_FPoint> normalised;
    for (const SDL_FPoint& p: yetPoints)
    {
        float fx = ((p.x + x) / 16) * 44;
        float fy = ((p.y + y) / 16) * 44;
        fx -= WindowManager::Instance()->camera.x;
        fy -= WindowManager::Instance()->camera.y;
        SDL_FPoint fp {fx, fy};
        normalised.emplace_back(fp);
    }
    drawPoly(normalised, "blue");

    SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 0, 0);
    SDL_RenderPresent(WindowManager::Instance()->renderer);
}

bool Game::isRunning()
{
    return running;
}
