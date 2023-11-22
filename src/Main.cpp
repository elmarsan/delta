// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "Game.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "absl/status/status.h"

#include <SDL2/SDL.h>

int main()
{
    // TODO: Timer
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

        // TODO: Timer
        frameTime = SDL_GetTicks64() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}
