#pragma once

#include "engine/Asset.h"

#include <memory>

class AssetLoader
{
  public:
    // virtual std::shared_ptr<Asset> load(const std::string& assetID) = 0;
    virtual std::shared_ptr<Asset> load(const std::string& assetID, AssetMetadata* metadata) = 0;
};

class TextureLoader: public AssetLoader
{
  public:
    // std::shared_ptr<Asset> load(const std::string& assetID); 
    std::shared_ptr<Asset> load(const std::string& assetID, AssetMetadata* metadata);
};
