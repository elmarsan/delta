#include "TileManager.h"
#include "ColliderComponent.h"
#include "TileComponent.h"
#include "WorldManager.h"
#include "absl/strings/str_format.h"
#include "delta/Game.h"
#include "ECS.h"
#include "engine/Animation.h"
#include "engine/SpriteComponent.h"
#include "engine/TransformComponent.h"
#include "engine/Asset.h"

extern Manager manager;

void TileManager::addTile(Vector2 gridPos, Tile tile) 
{
    auto& tileEntity(manager.addEntity());
    tileEntity.addGroup(Game::groupMap);
    tileEntity.addComponent<TileComponent>(gridPos, tile);

    if (tile.collides)
    {
        tileEntity.addComponent<TransformComponent>(gridPos, 44, 44, 1);
        tileEntity.addComponent<ColliderComponent>("tile");
        tileEntity.addGroup(Game::groupCollider);
    }
}

void TileManager::destroyMapTiles(WorldMap map)
{
    LOG(INFO) << absl::StrFormat("Destroying tiles of map: %s", map.getID());

    auto tiles = manager.getGroup(Game::groupMap);
    for (auto& tile: tiles)
    {
        auto pos = tile->getComponent<TileComponent>().gridPoint2;
        if (map.pointIn(pos))
        {
            LOG(INFO) << absl::StrFormat("Tile of map %s must be delete", map.getID());
        }
    }
}
