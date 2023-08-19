#pragma once

#include "ECS.h"
#include "TextureManager.h"
#include "Vector2D.h"

#include <map>
#include <string>

class AssetManager
{
  public:
      AssetManager(Manager* manager);
      ~AssetManager();

      void addTexture(std::string id, std::string path, SDL_Color* colorMod = nullptr);
      SDL_Texture* getTexture(std::string id);
  private:
      Manager* manager;
      std::map<std::string, SDL_Texture*> textures; 
};
