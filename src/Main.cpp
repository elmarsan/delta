#include "Game.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "absl/status/status.h"
#include "src/Polygon.h"
#include "src/Vec2.h"

#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>

int main()
{
    auto p = Polygon<int>(PolVertex<int> { Point2(1, 1), Point2(4, 5), Point2(7, 8), Point2(1, 8) });
    Point2 testPoint = Point2( 3, 4 );

    // Verificar si el punto está dentro del polígono
    if (p.isInside(testPoint))
    {
        std::cout << "El punto esta dentro del poligono.";
    }
    else
    {
        std::cout << "El punto esta fuera del poligono.";
    }
    // abort();

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
