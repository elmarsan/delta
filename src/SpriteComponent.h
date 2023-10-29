#pragma once

#include "Animation.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "Vector2.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "Game.h"

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
    int w, h;

    Point2 framePosition;
    std::map<std::string, Animation*> animations;
    std::string currentAnimation;

  public:
    SpriteComponent(int w, int h, std::string textureId): w(w), h(h)
    {
        texture = Game::assetManager->get<Texture>(textureId);
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

        src.x = framePosition.x;
        src.y = framePosition.y;
        src.w = w;
        src.h = h;
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
            framePosition = animation->getFramePos(frame);
        }
    }

    void stopAnimation()
    {
        if (currentAnimation == "")
            return;

        auto animation = animations[currentAnimation];
        if (animation != nullptr)
        {
            framePosition = animation->getFramePos(0);
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
            framePosition = animation->getFramePos(index);
            animation->nextFrame();
        }
    }
};
