#pragma once

#include "engine/Vector2D.h"
#include <SDL.h>

struct Texture
{
    SDL_Texture* sdlTexture;
    SDL_RendererFlip flip;
    Vector2D pos;
};
