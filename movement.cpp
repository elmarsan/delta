#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <stdio.h>
#include <string>

const int width = 640;
const int height = 480;

enum Direction { UP, DOWN, LEFT, RIGHT };

class Player {
public:
  Player(SDL_Texture *texture, SDL_Rect *world)
      : texture(texture), world(world) {
    x = 0;
    y = 0;
    direction = DOWN;
  }
  ~Player() = default;

  int x;
  int y;

public:
  void Move(Direction direction) {
    switch (direction) {
    case UP:
      Up();
      break;
    case DOWN:
      Down();
      break;
    case LEFT:
      Left();
      break;
    case RIGHT:
      Right();
      break;
    }
  }

private:
  void Left() {
    // if (x > 0) {
      x--;
    // }
  }
  void Right() {
    // if ( world->x) {
      x++;
    // }
  }
  void Up() {
    y++;
  }
  void Down() {
    y--;
  }

private:
  SDL_Texture *texture;
  SDL_Rect *world;

  Direction direction;
};

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

  // if (SDL_SetTextureColorMod(texture, 255, 0, 228) < 0) {
  //   printf("Unable to modulate texture magenta: %s\n", SDL_GetError());
  //   return nullptr;
  // }

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
    exit(1);
  }

  SDL_Texture *texture = loadTexture(renderer, "p1.png");
  if (texture == nullptr) {
    exit(1);
  }

  bool running = true;
  SDL_Rect world{0, 0, width, height};
  Player player = Player(texture, &world);

  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        running = false;
      }

      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
          printf("%s\n", "UP");
          player.Move(UP);
          break;

        case SDLK_DOWN:
          printf("%s\n", "DOWN");
          player.Move(DOWN);
          break;

        case SDLK_LEFT:
          printf("%s\n", "LEFT");
          player.Move(LEFT);
          break;

        case SDLK_RIGHT:
          printf("%s\n", "RIGHT");
          player.Move(RIGHT);
          break;
        }
      }
    }

    SDL_Rect playerRect{player.x, player.y, 14, 22};

    // Frame
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &playerRect, &playerRect);
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}
