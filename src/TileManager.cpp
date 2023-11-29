// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "TileManager.h"

#include "ColliderComponent.h"
#include "ECS.h"
#include "TileComponent.h"
#include "WorldManager.h"
#include "absl/strings/str_format.h"
#include "Game.h"
#include "Animation.h"
#include "Asset.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

extern Manager manager;

void TileManager::addTile(Point2 gridPos, Tile tile)
{
    auto tileEntity(manager.addEntity());
    tileEntity->addGroup(Game::groupMap);
    tileEntity->addComponent<TileComponent>(gridPos, tile);

    if (tile.collides)
    {
        tileEntity->addComponent<TransformComponent>(gridPos);
        tileEntity->addComponent<ColliderComponent>("tile");
        tileEntity->addGroup(Game::groupCollider);
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
