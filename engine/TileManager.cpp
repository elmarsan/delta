#include "TileManager.h"
#include "delta/Game.h"
#include "ECS.h"
#include "TileComponent.h"
#include "engine/AnimatedTileComponent.h"
#include "engine/Animation.h"
#include "engine/SpriteComponent.h"
#include "engine/TransformComponent.h"

extern Manager manager;

void TileManager::addTile(Vector2D gridPos, Texture texture)
{
    auto& tileEntity(manager.addEntity());
    tileEntity.addGroup(Game::groupMap);
    tileEntity.addComponent<TileComponent>(gridPos, texture);
}

void TileManager::addAnimatedTile(Vector2D gridPos, std::string tilesetID, std::vector<int> tileIDs, int speed)
{
    auto& tileEntity(manager.addEntity());
    tileEntity.addGroup(Game::groupMap);
    tileEntity.addComponent<AnimatedTileComponent>(gridPos, tilesetID, tileIDs, speed);
}
