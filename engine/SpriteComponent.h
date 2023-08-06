#pragma once

#include "ECS.h"
#include "SDL_render.h"
#include "TextureManager.h"
#include "TransformComponent.h"

#include <SDL.h>

class SpriteComponent: public Component
{
  private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_RendererFlip textureFlip;

    SDL_Rect srcRect;
    SDL_Rect dstRect;

  public:
    SpriteComponent(const std::string texturePath, SDL_Color* color = nullptr): textureFlip(SDL_FLIP_NONE)
    {
        texture = TextureManager::load(texturePath, color);
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        srcRect = SDL_Rect { 0, 0, 14, 21 };
        dstRect = SDL_Rect { 0, 0, 64, 64 };
    }

    void update() override
    {
        dstRect.x = transform->position.x;
        dstRect.y = transform->position.y;

        std::cout << transform->direction << std::endl;
        switch (transform->direction)
        {
            case Direction::DOWN: {
                srcRect.x = 0;
                textureFlip = SDL_FLIP_NONE;
            }
            case Direction::UP: {
                srcRect.x = 15;
                textureFlip = SDL_FLIP_NONE;
            }
            case Direction::LEFT: {
                srcRect.x = 30;
                textureFlip = SDL_FLIP_NONE;
            }
            case Direction::RIGHT: {
                srcRect.x = 30;
                textureFlip = SDL_FLIP_HORIZONTAL;
            }
        }
    }

    void draw() override { TextureManager::draw(texture, srcRect, dstRect, textureFlip); }
    // void setFlip(SDL_RendererFlip flip) { textureFlip = flip; }
};
