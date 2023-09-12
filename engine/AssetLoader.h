#pragma once

#include "absl/status/statusor.h"
#include "engine/Asset.h"

#include <memory>


class AssetLoader
{
  public:
    virtual absl::StatusOr<std::shared_ptr<Asset>> load(const std::string& assetID, AssetMetadata* metadata) = 0;
};

class TextureLoader: public AssetLoader
{
  public:
    absl::StatusOr<std::shared_ptr<Asset>> load(const std::string& assetID, AssetMetadata* metadata);
};
