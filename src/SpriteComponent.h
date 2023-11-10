#pragma once

#include "Animation.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "Vector2.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "Game.h"
#include "src/Asset.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <map>
#include <string_view>

class SpriteComponent: public Component
{
  private:
    TransformComponent* transform;
    std::shared_ptr<Texture> texture;
    SDL_Rect src, dst;
    Size2 size2;
    Point2 textureSrc;

    std::map<std::string, Animation*> animations;
    std::string currentAnimation;

  public:
    SpriteComponent(AssetID textureID, Size2 size2): size2(size2)
    {
        auto textureLoadRes = Game::assetManager->getOrLoad<Texture>(textureID);
        LOG_IF(ERROR, !textureLoadRes.ok()) << textureLoadRes.status().message();

        texture = textureLoadRes.value();
        animations = std::map<std::string, Animation*>();
    }

    SpriteComponent(AssetID textureID, Size2 size2, Point2 textureSrc): size2(size2), textureSrc(textureSrc)
    {
        auto textureLoadRes = Game::assetManager->getOrLoad<Texture>(textureID);
        LOG_IF(ERROR, !textureLoadRes.ok()) << textureLoadRes.status().message();

        texture = textureLoadRes.value();
        animations = std::map<std::string, Animation*>();
    }

    void addAnimation(std::string name, Animation* animation) { animations[name] = animation; }

    void init() override { transform = &entity->getComponent<TransformComponent>(); }

    void update() override
    {
        if (isAnimated())
            playAnimation();

        dst.x = transform->point2.x - WindowManager::Instance()->camera.x;
        dst.y = transform->point2.y - WindowManager::Instance()->camera.y;
        dst.w = dst.h = 44;

        src.x = textureSrc.x;
        src.y = textureSrc.y;
        src.w = size2.w;
        src.h = size2.h;
    }

    void draw() override { WindowManager::Instance()->renderTexture(texture, &src, &dst); }

    void setAnimation(const std::string& name, SDL_RendererFlip animFlip = SDL_FLIP_NONE)
    {
        currentAnimation = name;
        texture->flip = animFlip;
    }

    void setAnimationFrame(const std::string& name, int frame, SDL_RendererFlip animFlip = SDL_FLIP_NONE)
    {
        if (animations.size() == 0 || frame > animations.size() - 1 || frame < 0)
            return;

        auto animation = animations[name];
        if (animation != nullptr)
        {
            texture->flip = animFlip;
            textureSrc = animation->getFramePos(frame);
        }
    }

    void stopAnimation()
    {
        if (currentAnimation == "")
            return;

        auto animation = animations[currentAnimation];
        if (animation != nullptr)
        {
            textureSrc = animation->getFramePos(0);
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
            textureSrc = animation->getFramePos(index);
            animation->nextFrame();
        }
    }
};
