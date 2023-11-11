#pragma once

#include "Asset.h"
#include "AssetLoader.h"
#include "absl/log/log.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>

class AssetManager
{
  public:
    AssetManager()
    {
        loaders[typeid(Texture).name()] = std::make_shared<TextureLoader>();
        loaders[typeid(Tileset).name()] = std::make_shared<TilesetLoader>();
        loaders[typeid(Map).name()] = std::make_shared<MapLoader>();
    }

    template <typename T>
    absl::StatusOr<std::shared_ptr<T>> load(const std::string& assetID, AssetMetadata* metadata = nullptr)
    {
        const std::string typeName = typeid(T).name();
        if (!std::is_base_of<Asset, T>::value)
            return absl::InvalidArgumentError(absl::StrFormat("Type: %s is not an Asset", typeName));

        auto loader = loaders.find(typeName);
        auto noLoaderErr = absl::UnavailableError(absl::StrFormat("No asset loader found for: %s", typeName));
        if (loader == loaders.end())
            return noLoaderErr;

        auto asset = loader->second->load(assetID, metadata);
        if (!asset.ok())
            return asset.status();

        assets[assetID] = asset.value();
        return std::dynamic_pointer_cast<T>(asset.value());
    }

    template <typename T>
    std::shared_ptr<T> get(const std::string& assetID) const
    {
        auto it = assets.find(assetID);
        if (it != assets.end())
        {
            auto asset = std::dynamic_pointer_cast<T>(it->second);
            if (asset)
                return asset;
        }
        return nullptr;
    }

    template <typename T>
    absl::StatusOr<std::shared_ptr<T>> getOrLoad(const std::string& assetID,
                                                 AssetMetadata* metadata = nullptr)
    {
        std::shared_ptr<T> asset = get<T>(assetID);
        if (asset == nullptr)
        {
            auto loadRes = load<T>(assetID);
            if (loadRes.ok())
            {
                assets[assetID] = loadRes.value();
                std::shared_ptr<T> asset = get<T>(assetID);
            }
            return loadRes;
        }

        return asset;
    }

  private:
    std::map<std::string, std::shared_ptr<AssetLoader>> loaders;
    std::map<std::string, std::shared_ptr<Asset>> assets;
};
