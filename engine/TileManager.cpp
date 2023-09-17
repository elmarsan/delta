#include "TileManager.h"
#include "ColliderComponent.h"
#include "TileComponent.h"
#include "delta/Game.h"
#include "ECS.h"
#include "engine/Animation.h"
#include "engine/SpriteComponent.h"
#include "engine/TransformComponent.h"
#include "engine/Asset.h"

extern Manager manager;

void TileManager::addTile(Vector2D gridPos, Tile tile) 
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
