#include "TextureManager.h"

#include "SDL_pixels.h"
#include "SDL_stdinc.h"
#include "SDL_surface.h"

#include <SDL.h>
#include <SDL_image.h>
#include <string>

SDL_Texture* TextureManager::load(std::string path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* TextureManager::load(std::string path, SDL_Renderer* renderer, SDL_Color colorMod)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    auto modulation = SDL_MapRGB(surface->format, colorMod.r, colorMod.g, colorMod.b);
    SDL_SetColorKey(surface, SDL_TRUE, modulation);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
