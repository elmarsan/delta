#include "Game.h"

#include <SDL2/SDL.h>
#include <iostream>

int main()
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint64 frameStart;
    int frameTime;

    Game game;
    if (!game.init(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640))
    {
        std::cout << "Unable to init SDL" << std::endl;
        return 1;
    }

    while (game.isRunning())
    {
        frameStart = SDL_GetTicks64();

        game.handleEvents();
        game.update();
        game.render();

        frameTime = SDL_GetTicks64() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game.clean();
}
