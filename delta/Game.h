#pragma once

#include "engine/AssetManager.h"

#include <SDL2/SDL_events.h>
#include <absl/status/status.h>
#include <memory>
#include <string>
#include <vector>

class Game
{
  public:
    Game() = default;
    absl::Status init();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool isRunning();

    static SDL_Event event;
    static std::unique_ptr<AssetManager> assetManager;
    enum groupLabels : std::size_t
    {
        groupMap,
        groupPlayer,
        groupCollider,
        groupTeleport
    };

  private:
    bool running;
};
