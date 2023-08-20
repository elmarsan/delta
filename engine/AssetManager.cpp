#include "AssetManager.h"

#include "SDL_render.h"
#include "engine/TextureManager.h"

AssetManager::AssetManager(Manager* manager): manager(manager)
{
}

AssetManager::~AssetManager()
{
    for(const auto& pair: textures)
    {
        SDL_DestroyTexture(pair.second);
    }
}

SDL_Texture* AssetManager::addTexture(std::string id, std::string path, SDL_Color* colorMod)
{
    textures[id] = TextureManager::load(path, colorMod);
    return textures[id];
}

SDL_Texture* AssetManager::getTexture(std::string id)
{
    return textures[id];
}
