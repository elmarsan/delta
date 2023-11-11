#include "Player.h"

void Player::init()
{
    // Load player texture
    SDL_Color colorMod { 255, 0, 228 };
    AssetMetadata meta;
    meta = TextureMetadata { &colorMod };
    auto loadTextureRes = Game::assetManager->load<Texture>("p1", &meta);
    LOG_IF(ERROR, !loadTextureRes.ok()) << loadTextureRes.status().message();

    transform = &addComponent<TransformComponent>(Point2(440, 396));
    sprite = &addComponent<SpriteComponent>("p1", Size2(14, 21));
    controller = &addComponent<CharacterController>();
    collider = &addComponent<ColliderComponent>("Player");
    addGroup(Game::groupPlayer);
    addGroup(Game::groupCollider);

    initAnimations();
}

void Player::initAnimations()
{
    sprite->addAnimation(
        "walk_up", new Animation(150, std::vector<Point2> { Point2(15, 0), Point2(15, 22), Point2(15, 44) }));
    sprite->addAnimation(
        "walk_down", new Animation(150, std::vector<Point2> { Point2(0, 0), Point2(0, 22), Point2(0, 44) }));
    sprite->addAnimation(
        "walk_lateral",
        new Animation(150, std::vector<Point2> { Point2(30, 0), Point2(31, 22), Point2(30, 44) }));
}
