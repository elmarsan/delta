#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>

const int width = 840;
const int height = 680;

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

// Modulation RGB 255,0,228
SDL_Texture *loadTexture(SDL_Renderer *renderer, std::string path) {
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (surface == nullptr) {
    printf("Unable to load img: %s\n", SDL_GetError());
    return nullptr;
  }

  auto colorModulation = SDL_MapRGB(surface->format, 255, 0, 228);
  if (SDL_SetColorKey(surface, SDL_TRUE, colorModulation) < 0) {
    printf("Unable to modulate surface magenta: %s\n", SDL_GetError());
    return nullptr;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (surface == nullptr) {
    printf("Unable to load img: %s\n", SDL_GetError());
    return nullptr;
  }

  delete surface;
  return texture;
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Unable to initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }
  if (IMG_Init(IMG_INIT_PNG) < 0) {
    printf("Unable to initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_Window *window =
      SDL_CreateWindow("movement", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("Unable to create window: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  if (window == nullptr) {
    printf("Unable to create renderer: %s\n", SDL_GetError());
  }

  SDL_Texture *texture = loadTexture(renderer, "p1.png");
  if (texture == nullptr) {
    exit(1);
  }

  bool running = true;
  SDL_Event event;

  int playerXPos = 0;
  int playerYPos = 0;
  int inc = 12;

  int playerX = 0;
  int playerY = 0;

  while (running) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        running = false;
      }

      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
          playerX = 15;
          playerYPos -= inc;
          break;
        case SDLK_DOWN:
          playerX = 0;
          playerYPos += inc;
          break;
        case SDLK_LEFT:
          playerX = 30;
          playerXPos -= inc;
          break;
        case SDLK_RIGHT:
          playerXPos += inc;
          break;
        }
      }
    }
    SDL_Rect playerRect{playerX, playerY, 14, 21};
    SDL_Rect worldRect{playerXPos, playerYPos, playerRect.w * 3,
                       playerRect.h * 3};

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &playerRect, &worldRect);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}
