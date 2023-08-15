#pragma once

#include <SDL.h>
#include <vector>

class ColliderComponent;

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

    static void addTile(int id, int x, int y);
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent*> colliders;

  private:
    bool running;
    SDL_Window* window;
};
