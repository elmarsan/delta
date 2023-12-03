// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "ActorManager.h"
#include "MapManager.h"
#include "RenderManager.h"
#include "SDL.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "TerrainManager.h"
#include "TilesetManager.h"
#include "WindowManager.h"

#include <SDL2/SDL_image.h>
#include <memory>

class Engine
{
  public:
    ~Engine()
    {
        SDL_DestroyRenderer(sdlRenderer);
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit(); 
    }

    static inline void init()
    {
        DLOG(INFO) << "Initializing SDLWindowSystem...";

        LOG_IF(FATAL, SDL_Init(SDL_INIT_VIDEO) != 0) << "Unable to init sdl video";
        LOG_IF(FATAL, IMG_Init(IMG_INIT_PNG) == 0) << "Unable to init sdl image";

        sdlWindow = SDL_CreateWindow(
            "Delta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 840, 680, SDL_WINDOW_SHOWN);
        LOG_IF(FATAL, sdlWindow == nullptr) << "Unable to create window";

        sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_SOFTWARE);
        LOG_IF(FATAL, sdlRenderer == nullptr) << "Unable to create renderer";

        windowManager = std::make_unique<SDLWindowManager>(sdlWindow);
        renderManager = std::make_unique<SDLRenderManager>(sdlRenderer);
        actorManager = std::make_unique<DeltaActorManager>();
        terrainManager = std::make_unique<DeltaTerrainManager>();
        tilesetManager = std::make_unique<TiledTilesetManager>();
        mapManager = std::make_unique<DeltaMapManager>();
    }

    [[nodiscard]] static inline WindowManager& window() { return *windowManager.get(); }
    [[nodiscard]] static inline ActorManager& actor() { return *actorManager.get(); }
    [[nodiscard]] static inline RenderManager& render() { return *renderManager.get(); }
    [[nodiscard]] static inline TerrainManager& terrain() { return *terrainManager.get(); }
    [[nodiscard]] static inline TilesetManager& tileset() { return *tilesetManager.get(); }
    [[nodiscard]] static inline MapManager& map() { return *mapManager.get(); }

  private:
    static std::unique_ptr<WindowManager> windowManager;
    static std::unique_ptr<ActorManager> actorManager;
    static std::unique_ptr<RenderManager> renderManager;
    static std::unique_ptr<TerrainManager> terrainManager;
    static std::unique_ptr<TilesetManager> tilesetManager;
    static std::unique_ptr<MapManager> mapManager;

    static SDL_Window* sdlWindow;
    static SDL_Renderer* sdlRenderer;
};
