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
    ~Game();
    absl::Status init(int x, int y, int width, int height);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool isRunning();

    static SDL_Renderer* renderer;
    static SDL_Event event;
    static std::unique_ptr<AssetManager> assetManager;
    // static AssetManager* assets;
    enum groupLabels : std::size_t
    {
        groupMap,
        groupPlayer,
        groupCollider,
        groupTeleport
    };

  private:
    bool running;
    SDL_Window* window;
};
