#include "Game.h"

#include "Player.h"
#include "SDL_render.h"
#include "TextureManager.h"

#include <SDL2/SDL.h>
#include <iostream>

Player* player = nullptr;

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

    SDL_Color modulation { 255, 0, 228 };
    SDL_Texture* playerTexture = TextureManager::load("data/p1.png", renderer, modulation);
    player = new Player(playerTexture);
    return true;
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT: running = false;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym)
            {
                case SDLK_UP: {
                    std::cout << "Up" << std::endl;
                    player->up();
                    break;
                }
                case SDLK_DOWN: {
                    std::cout << "Down" << std::endl;
                    player->down();
                    break;
                }
                case SDLK_LEFT: {
                    std::cout << "Left" << std::endl;
                    player->left();
                    break;
                }
                case SDLK_RIGHT: {
                    std::cout << "Right" << std::endl;
                    player->right();
                    break;
                }
            }
            break;
        }
    }
}
void Game::update()
{
    // xPos += 4;
    // yPos++;
    // rect.h = 64;
    // rect.w = 64;
    // rect.x = xPos;
    // rect.y = yPos;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    player->render(renderer);
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
