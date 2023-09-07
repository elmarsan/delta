#include "AssetLoader.h"

#include "delta/Game.h"
#include "engine/Asset.h"

#include <SDL.h>
#include <SDL_image.h>
#include <absl/strings/str_format.h>
#include <filesystem>
#include <memory>

// std::shared_ptr<Asset> TextureLoader::load(const std::string& assetID)
std::shared_ptr<Asset> TextureLoader::load(const std::string& assetID, AssetMetadata* metadata)
{
    std::string path = absl::StrFormat("data/assets/%s.png", assetID);
    if (!std::filesystem::exists(path))
    {
        return nullptr;
        // return absl::NotFoundError(absl::StrFormat("Map: %s not found", mapID));
    }
    // Error handling while checking std::variant
    // SDL_Color* colorMod = std::get<TextureMetadata>(metadata).colorMod;
    SDL_Surface* surface = IMG_Load(path.c_str());
    // if (colorMod != nullptr)
    // {
    //     auto modulation = SDL_MapRGB(surface->format, colorMod->r, colorMod->g, colorMod->b);
    //     SDL_SetColorKey(surface, SDL_TRUE, modulation);
    //     colorMod = NULL;
    //     delete colorMod;
    // }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return std::make_shared<TextureV2>(assetID, texture);
}
