#pragma once

#include "Animation.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "Vector2D.h"
#include "delta/Game.h"

#include <SDL.h>
#include <map>
#include <string_view>

class SpriteComponent: public Component
{
  private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_RendererFlip flip;
    SDL_Rect src, dst;
    int w, h;

    Vector2D framePosition;
    std::map<std::string, Animation> animations;
    std::string currentAnimation;

  public:
    SpriteComponent(int w, int h, std::string textureId): flip(SDL_FLIP_NONE), w(w), h(h)
    {
        texture = Game::assets->getTexture(textureId);
        animations = std::map<std::string, Animation>();
    }

    void addAnimation(std::string name, const Animation& animation) { animations[name] = animation; }

    void init() override { transform = &entity->getComponent<TransformComponent>(); }

    void update() override
    {
        if (isAnimated())
            playAnimation();

        dst.x = transform->position.x;
        dst.y = transform->position.y;
        dst.w = dst.h = 44;

        src.x = framePosition.x;
        src.y = framePosition.y;
        src.w = w;
        src.h = h;
    }

    void draw() override { TextureManager::draw(texture, &src, &dst, flip); }

    void setAnimation(const std::string& name, SDL_RendererFlip animFlip = SDL_FLIP_NONE)
    {
        currentAnimation = name;
        flip = animFlip;
    }

    void stopAnimation()
    {
        if (currentAnimation != "")
        {
            auto animation = animations[currentAnimation];
            framePosition = animation.getFramePos(0);
            currentAnimation = "";
        }
    }

    bool isAnimated() const { return animations.size() > 0 && currentAnimation != ""; }

    void playAnimation()
    {
        if (currentAnimation != "")
        {
            auto animation = animations[currentAnimation];
            int numFrames = animation.numFrames();
            int speed = animation.speed;

            int index = static_cast<int>((SDL_GetTicks() / speed) % numFrames);
            framePosition = animation.getFramePos(index);
            animation.nextFrame();
        }
    }

    void setFlip(SDL_RendererFlip newFlip) { flip = newFlip; }
};
