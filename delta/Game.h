#pragma once

#include "engine/AssetManager.h"
#include <SDL.h>
#include <string>
#include <vector>

class Game
{
  public:
    Game() = default;
    ~Game();
    bool init(int x, int y, int width, int height);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool isRunning();
    void addTile(int x, int y, int w, int h, std::string texturePath);

    static SDL_Renderer* renderer;
    static SDL_Event event;
    static AssetManager* assets;
    enum groupLabels : std::size_t
    {
        groupMap,
        groupPlayer,
        groupCollider
    };

  private:
    bool running;
    SDL_Window* window;
};
