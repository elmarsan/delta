#include "Player.h"

#include "SDL_render.h"

#include <SDL2/SDL.h>

Player::Player(SDL_Texture* texture):
    texture(texture), textureFlip(SDL_FLIP_NONE), moveSize(12), xTexture(0), yTexture(0)
{
}

Player::~Player()
{
    SDL_DestroyTexture(texture);
}

void Player::up()
{
    yPos -= moveSize;
    xTexture = 15;
}

void Player::down()
{
    yPos += moveSize;
    xTexture = 0;
}

void Player::left()
{
    xPos -= moveSize;
    xTexture = 30;
    textureFlip = SDL_FLIP_NONE;
}

void Player::right()
{
    xPos += moveSize;
    xTexture = 30;
    textureFlip = SDL_FLIP_HORIZONTAL;
}

void Player::render(SDL_Renderer* renderer)
{
    SDL_Rect clip { xTexture, yTexture, 14, 21 };
    SDL_Rect textureRect { xPos, yPos, clip.w * 3, clip.h * 3 };
    SDL_RenderCopyEx(renderer, texture, &clip, &textureRect, 0, nullptr, textureFlip);
}
