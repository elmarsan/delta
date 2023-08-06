#include "Game.h"

#include "SDL_image.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "engine/ECS.h"
#include "engine/KeyboardControllerComponent.h"
#include "engine/SpriteComponent.h"
#include "engine/TextureManager.h"
#include "engine/TransformComponent.h"

#include <SDL2/SDL.h>
#include <iostream>

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager;
auto& newPlayer(manager.addEntity());

Game::~Game()
{
    clean();
}

bool Game::init(int x, int y, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return 0;
    if (IMG_Init(IMG_INIT_PNG) == 0)
        return 0;
    window = SDL_CreateWindow(
        "Delta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    running = true;

    newPlayer.addComponent<TransformComponent>();
    SDL_Color modulation { 255, 0, 228 };
    newPlayer.addComponent<SpriteComponent>("data/p1.png", &modulation);
    newPlayer.addComponent<KeyboardControllerComponent>();
    return true;
}

void Game::handleEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event) != 0)
    {
        Game::event = event;

        if (event.type == SDL_QUIT)
        {
            running = false;
        }
    }
}
void Game::update()
{
    manager.refresh();
    manager.update();
}

void Game::render()
{
    SDL_RenderClear(renderer);
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

bool Game::isRunning()
{
    return running;
}
