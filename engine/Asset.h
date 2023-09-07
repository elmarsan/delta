#pragma once

#include <SDL.h>
#include <string>
#include <variant>

class Asset
{
  public:
    virtual std::string getID() = 0;
};

class TextureV2: public Asset
{
  public:
    SDL_Texture* sdlTexture;
    SDL_RendererFlip flip;

    TextureV2(std::string ID, SDL_Texture* sdlTexture): ID(ID), sdlTexture(sdlTexture) {}

    std::string getID() override { return ID; }

  private:
    std::string ID;
};

struct TextureMetadata
{
    SDL_Color* colorMod;

    ~TextureMetadata() { delete colorMod; }
};

using AssetMetadata = std::variant<TextureMetadata>;
