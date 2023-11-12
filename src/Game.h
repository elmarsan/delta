#pragma once

#include "AssetManager.h"

#include <absl/status/status.h>
#include <memory>
#include <string>

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

    static std::unique_ptr<AssetManager> assetManager;
    enum groupLabels : std::size_t
    {
        groupMap,
        groupPlayer,
        groupCollider,
        groupNpc
    };

    static void lockCharacterControllers();
    static void unlockCharacterControllers();

  private:
    bool running;
};
