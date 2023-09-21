#pragma once

#include "Animation.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "Vector2D.h"
#include "WindowManager.h"
#include "delta/Game.h"
#include "absl/log/log.h"

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

    Vector2D framePosition;
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
       
        dst.x = transform->position.x - WindowManager::Instance()->camera.x;
        dst.y = transform->position.y - WindowManager::Instance()->camera.y;        // dst.x = transform->position.x; 
        dst.w = dst.h = 44;

        src.x = framePosition.x;
        src.y = framePosition.y;
        src.w = w;
        src.h = h;
    }

    void draw() override 
    { 
        WindowManager::Instance()->renderTexture(texture, &src, &dst);
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 0xff, 0);
        dst.w += 10;
        SDL_RenderDrawRect(WindowManager::Instance()->renderer, &dst);
        dst.w -= 10;
        SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 0, 0);
    } 

    void setAnimation(const std::string& name, SDL_RendererFlip animFlip = SDL_FLIP_NONE)
    {
        currentAnimation = name;
        texture->flip = animFlip;
    }

    void stopAnimation()
    {
        if (currentAnimation != "")
        {
            auto animation = animations[currentAnimation];
            framePosition = animation->getFramePos(0);
            currentAnimation = "";
        }
    }

    bool isAnimated() const { return animations.size() > 0 && currentAnimation != ""; }

    void playAnimation()
    {
        if (currentAnimation != "")
        {
            auto animation = animations[currentAnimation];
            int numFrames = animation->numFrames();
            int speed = animation->speed;

            int index = static_cast<int>((SDL_GetTicks() / speed) % numFrames);
            framePosition = animation->getFramePos(index);
            animation->nextFrame();
        }
    }
};
