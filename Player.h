#pragma once

#include <SDL2/SDL.h>
#include <string>

/* 14 x 21
 * Sprites
 * Down frame 1 = 0 0
 * Down frame 2 = 0 22
 * Down frame 3 = 0 44
 *
 * Up frame 1 = 15 0
 * Up frame 2 = 15 22
 * Up frame 3 = 15 44
 *
 * Side frame 1 = 30 0
 * Side frame 2 = 31 22
 * Side frame 3 = 30 44
 */

class Player
{
  public:
    Player(SDL_Texture* texture);
    ~Player();

    void up();
    void down();
    void left();
    void right();

    void render(SDL_Renderer* renderer);

  private:
    SDL_Texture* texture;
    SDL_RendererFlip textureFlip;
    int moveSize;
    int xTexture;
    int yTexture;
    int xPos;
    int yPos;
};
