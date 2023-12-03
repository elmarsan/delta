// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Animation.h"
#include "ECS.h"
#include "Game.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "math/Vec2.h"
#include "src/Asset.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <map>

class SpriteComponent: public Component
{
  public:
    SpriteComponent(AssetID textureID, Size2 size2): size2(size2)
    {
        texture = Engine::render().getTexture(textureID);
        LOG_IF(ERROR, texture == nullptr) << absl::StrFormat("Texture '%s', not found", textureID);
        animations = std::map<std::string, Animation*>();
    }

    SpriteComponent(AssetID textureID, Size2 size2, Point2 textureSrc): size2(size2), textureSrc(textureSrc)
    {
        texture = Engine::render().getTexture(textureID);
        LOG_IF(ERROR, texture == nullptr) << absl::StrFormat("Texture '%s', not found", textureID);
        animations = std::map<std::string, Animation*>();
    }

    void addAnimation(std::string name, Animation* animation) { animations[name] = animation; }

    void init() override { transform = &entity->getComponent<TransformComponent>(); }

    void update() override
    {
        if (isAnimated())
            playAnimation();

        auto camPos = Engine::actor().getCameraPos();
        dst.x = transform->point2.x - camPos.x;
        dst.y = transform->point2.y - camPos.y;
        dst.w = dst.h = 44;

        src.x = textureSrc.x;
        src.y = textureSrc.y;
        src.w = size2.w;
        src.h = size2.h;
    }

    void draw() override
    {
        if (texture != nullptr)
        {
            Engine::render().addTexture(texture, &src, &dst, flip);
        }
    }

    void setAnimation(const std::string& name, SDL_RendererFlip animFlip = SDL_FLIP_NONE)
    {
        currentAnimation = name;
        flip = animFlip;
    }

    void setAnimationFrame(const std::string& name, int frame, SDL_RendererFlip animFlip = SDL_FLIP_NONE)
    {
        if (animations.size() == 0 || frame > animations.size() - 1 || frame < 0)
            return;

        auto animation = animations[name];
        if (animation != nullptr)
        {
            flip = animFlip;
            textureSrc = animation->getFramePoint2(frame);
        }
    }

    void stopAnimation()
    {
        if (currentAnimation == "")
            return;

        auto animation = animations[currentAnimation];
        if (animation != nullptr)
        {
            textureSrc = animation->getFramePoint2(0);
            currentAnimation = "";
        }
    }

    bool isAnimated() const { return animations.size() > 0 && currentAnimation != ""; }

    void playAnimation()
    {
        if (currentAnimation == "")
            return;

        auto animation = animations[currentAnimation];
        if (animation != nullptr)
        {
            int numFrames = animation->numFrames();
            int speed = animation->speed;
            int index = static_cast<int>((SDL_GetTicks() / speed) % numFrames);
            textureSrc = animation->getFramePoint2(index);
            animation->nextFrame();
        }
    }

  private:
    TransformComponent* transform;
    Texture* texture;
    SDL_RendererFlip flip;
    Rect src;
    Rect dst;
    Size2 size2;
    Point2 textureSrc;

    // TODO: Improve animation handling.
    std::map<std::string, Animation*> animations;
    std::string currentAnimation;
};
