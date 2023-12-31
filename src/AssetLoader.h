// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "absl/status/statusor.h"
#include "Asset.h"

using AssetLoadResult = absl::StatusOr<std::shared_ptr<Asset>>;

class AssetLoader
{
  public:
    virtual AssetLoadResult load(const std::string& assetID, AssetMetadata* metadata) = 0;
};

class TextureLoader: public AssetLoader
{
  public:
    AssetLoadResult load(const std::string& assetID, AssetMetadata* metadata) override;
};

class TilesetLoader: public AssetLoader
{
  public:
    AssetLoadResult load(const std::string& assetID, AssetMetadata* metadata);
};

class MapLoader: public AssetLoader
{
  public:
    AssetLoadResult load(const std::string& assetID, AssetMetadata* metadata);
};
