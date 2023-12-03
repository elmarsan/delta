#include "Engine.h"
#include "ActorManager.h"
#include "MapManager.h"
#include "RenderManager.h"
#include "TerrainManager.h"
#include "TilesetManager.h"
#include "WindowManager.h"

std::unique_ptr<WindowManager> Engine::windowManager;
std::unique_ptr<RenderManager> Engine::renderManager;
std::unique_ptr<ActorManager> Engine::actorManager;
std::unique_ptr<TerrainManager> Engine::terrainManager;
std::unique_ptr<TilesetManager> Engine::tilesetManager;
std::unique_ptr<MapManager> Engine::mapManager;

SDL_Window* Engine::sdlWindow;
SDL_Renderer* Engine::sdlRenderer;
