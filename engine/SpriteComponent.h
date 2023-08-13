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

namespace engine
{
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

namespace component
{
    class Sprite: public Component
    {
      private:
        component::Transform* transform;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        SDL_Rect src, dst;
        engine::Sprite* sprite;
        int width, height;
        bool animated;

      public:
        int animIndex = 0;
        int sdlTicks;

        Sprite(engine::Sprite* sprite, int w, int h, bool animated):
            sprite(sprite), width(w), height(h), animated(animated)
        {
        }

        ~Sprite() = default;

        void init() override { transform = &entity->getComponent<Transform>(); }

        void update() override
        {
            dst.x = transform->position.x;
            dst.y = transform->position.y;
            dst.w = dst.h = 44;


            auto frame = sprite->getFrame(transform->direction, animIndex);
            if (animated)
            {
                animate();
            }
            else
            {
                src.x = frame.position.x;
                src.y = frame.position.y;
            }
            // auto frame = sprite->getFrame(transform->direction, animIndex);
            flip = frame.flip;
            src.w = width;
            src.h = height;

            sdlTicks = SDL_GetTicks();
        }

        void draw() override { TextureManager::draw(sprite->texture, &src, &dst, flip); }

        void animate()
        {
            int nframes = sprite->frames[transform->direction].size();
            if (animIndex < nframes - 1)
            {
                animIndex++;
            }
            else
            {
                animIndex = 0;
            }

            if (SDL_GetTicks() > sdlTicks + 100)
            {
                auto frame = sprite->getFrame(transform->direction, animIndex);
                src.x = frame.position.x;
                src.y = frame.position.y;
            }
        }
    };
}; // namespace component
}; // namespace engine
