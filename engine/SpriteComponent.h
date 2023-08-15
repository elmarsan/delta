#pragma once

#include "Animation.h"
#include "ECS.h"
#include "SDL_timer.h"
#include "TextureManager.h"
#include "TransformComponent.h"
#include "Vector2D.h"

#include <SDL.h>
#include <algorithm>
#include <map>
#include <memory>

struct FrameTexture
{
    Vector2D position;
    SDL_RendererFlip flip;
    SDL_Texture* texture;
};

using SpriteFrames = std::map<Direction, std::vector<Vector2D>>;
using SpriteFlips = std::map<Direction, SDL_RendererFlip>;

class Sprite
{
  public:
    SpriteFrames frames;
    SpriteFlips flips;
    SDL_Texture* texture;

    Sprite(SpriteFrames frames, std::string texturePath, SDL_Color* colorMod): frames(frames)
    {
        texture = TextureManager::load(texturePath, colorMod);
    }

    ~Sprite() { SDL_DestroyTexture(texture); }

    void flipDirection(Direction direction, SDL_RendererFlip flip) { flips[direction] = flip; }

    FrameTexture getFrame(Direction direction, int index)
    {
        return { frames[direction][index], getFlip(direction), texture };
    }

    SDL_RendererFlip getFlip(Direction direction)
    {
        auto it = flips.find(direction);
        if (it != flips.end())
        {
            return it->second;
        }
        return SDL_FLIP_NONE;
    }
};

class SpriteComponent: public Component
{
  private:
    TransformComponent* transform;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Rect src, dst;
    Sprite* sprite;
    int w, h;

  public:
    SpriteComponent(Sprite* sprite, int w, int h): sprite(sprite), w(w), h(h) {}

    ~SpriteComponent() = default;

    void init() override { transform = &entity->getComponent<TransformComponent>(); }

    void update() override
    {
        dst.x = transform->position.x;
        dst.y = transform->position.y;
        dst.w = dst.h = 44;

        auto frame = sprite->getFrame(transform->direction, 0);
        flip = frame.flip;
        src.x = frame.position.x;
        src.y = frame.position.y;
        src.w = w;
        src.h = h;
    }

    void draw() override { TextureManager::draw(sprite->texture, &src, &dst, flip); }
};
