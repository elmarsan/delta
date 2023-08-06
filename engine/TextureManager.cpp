#include "TextureManager.h"

#include "../Game.h"

#include <SDL.h>
#include <SDL_image.h>
#include <string>

SDL_Texture* TextureManager::load(std::string path, SDL_Color* colorMod)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (colorMod != nullptr)
    {
        auto modulation = SDL_MapRGB(surface->format, colorMod->r, colorMod->g, colorMod->b);
        SDL_SetColorKey(surface, SDL_TRUE, modulation);
        colorMod = NULL;
        delete colorMod;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::draw(SDL_Texture* texture,
                          SDL_Rect srcRect,
                          SDL_Rect dstRect,
                          SDL_RendererFlip renderFlip)
{
    SDL_RenderCopyEx(Game::renderer, texture, &srcRect, &dstRect, 0, NULL, renderFlip);
}
