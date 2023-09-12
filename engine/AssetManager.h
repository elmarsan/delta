#pragma once

#include "Asset.h"
#include "AssetLoader.h"
#include "absl/status/statusor.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>

class AssetManager
{
  public:
    AssetManager()
    {
        loaders[typeid(TextureV2).name()] = std::make_shared<TextureLoader>();
    }

    template <typename T>
    absl::StatusOr<std::shared_ptr<T>> load(const std::string& assetId, AssetMetadata* metadata = nullptr)
    {
        const std::string typeName = typeid(T).name();
        if (!std::is_base_of<Asset, T>::value)
        {
            return absl::InvalidArgumentError(absl::StrFormat("Type: %s is not an Asset", typeName));
        }

        auto loader = loaders.find(typeName);
        auto noLoaderErr = absl::UnavailableError(absl::StrFormat("No asset loader found for: %s", typeName));
        if (loader == loaders.end())
        {
            return noLoaderErr;
        }
        auto asset = loader->second->load(assetId, metadata);
        if (!asset.ok())
        {
            return asset.status();
        }

        assets[assetId] = asset.value();
        return std::dynamic_pointer_cast<T>(asset.value());
    }

    template <typename T>
    std::shared_ptr<T> get(const std::string& assetId) const
    {
        auto it = assets.find(assetId);
        if (it != assets.end())
        {
            auto asset = std::dynamic_pointer_cast<T>(it->second);
            if (asset)
            {
                return asset;
            }
        }
        return nullptr;
    }

  private:
    std::map<std::string, std::shared_ptr<AssetLoader>> loaders;
    std::map<std::string, std::shared_ptr<Asset>> assets;
};
