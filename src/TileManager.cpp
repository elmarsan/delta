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

void TileManager::addTile(Vector2 gridPos, Tile tile)
{
    auto& tileEntity(manager.addEntity());
    tileEntity.addGroup(Game::groupMap);
    tileEntity.addComponent<TileComponent>(gridPos, tile);

    if (tile.collides)
    {
        tileEntity.addComponent<TransformComponent>(gridPos, Size2(44, 44));
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

int TileManager::findClosestNumber(int num)
{
    if (num % 44 == 0)
        return num;

    int closestNumber = 0;
    int closestDifference = abs(num - closestNumber);

    for (int i = 44; i < num; i += 44)
    {
        int difference = abs(num - i);
        if (difference < closestDifference)
        {
            closestNumber = i;
            closestDifference = difference;
        }
    }

    return closestNumber;
}

Point2 TileManager::findClosestPoint2(Point2 point2)
{
    int closestX = findClosestNumber(point2.x);
    LOG(INFO) << closestX;
    int closestY = findClosestNumber(point2.y);
    return Point2(closestX, closestY);
}
