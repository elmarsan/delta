#include "absl/log/log.h"
#include "absl/status/status.h"
#include "Game.h"
#include "WindowManager.h"

#include <SDL2/SDL.h>
#include <iostream>

int main()
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint64 frameStart;
    int frameTime;

    absl::Status windowManagerStatus = WindowManager::init(840, 680);
    LOG_IF(FATAL, !windowManagerStatus.ok()) << windowManagerStatus.message();

    Game game;
    absl::Status status = game.init();
    LOG_IF(FATAL, !status.ok()) << status.message();

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
}
