#include "AssetLoader.h"

#include "Asset.h"
#include "delta/Game.h"
#include "nlohmann/json.hpp"
#include "absl/log/log.h"

#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <memory>
#include <variant>

AssetLoadResult TextureLoader::load(const std::string& assetID, AssetMetadata* metadata)
{
    LOG(INFO) << "Loading Texture: " << assetID;
    std::string path = absl::StrFormat("data/assets/%s.png", assetID);
    if (!std::filesystem::exists(path))
    {
        return absl::NotFoundError(absl::StrFormat("Texture: %s not found", assetID));
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (metadata != nullptr)
    {
        SDL_Color* colorMod = std::get<TextureMetadata>(*metadata).colorMod;
        if (colorMod != nullptr)
        {
            auto modulation = SDL_MapRGB(surface->format, colorMod->r, colorMod->g, colorMod->b);
            SDL_SetColorKey(surface, SDL_TRUE, modulation);
        }
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return std::make_shared<Texture>(assetID, texture);
}

AssetLoadResult TilesetLoader::load(const std::string& assetID, AssetMetadata* metadata)
{
    LOG(INFO) << "Loading Tileset: " << assetID;
    std::string path = absl::StrFormat("data/tiles/export/%s.json", assetID);
    if (!std::filesystem::exists(path))
    {
        return absl::NotFoundError(absl::StrFormat("Tileset: %s not found", assetID));
    }

    std::ifstream f("data/tiles/export/" + assetID + ".json");
    nlohmann::json data = nlohmann::json::parse(f);

    // Tileset tileset(assetID);
    Tileset tileset;
    tileset.setID(assetID);
    tileset.columns = data["columns"];
    tileset.margin = data["margin"];
    tileset.spacing = data["spacing"];
    tileset.numTiles = data["tilecount"];
    tileset.tileWidth = data["tilewidth"];
    tileset.tileHeight = data["tilewidth"];

    std::string image = data["image"];
    size_t lastSlash = image.find_last_of("/\\");
    std::string filename = image.substr(lastSlash + 1);
    size_t lastDot = filename.find_last_of(".");
    std::string textureID = filename.substr(0, lastDot);

    // TODO Load asset texture
    tileset.textureID = textureID;

    // Load animations and colliders
    for (auto& tile: data["tiles"])
    {
        auto tileAnimation = tile["animation"];
        if (!tileAnimation.is_null())
        {
            auto frames = std::vector<int>();
            int speed;
            for (auto& frame: tileAnimation)
            {
                frames.emplace_back(frame["tileid"]);
                speed = frame["duration"];
            }

            tileset.tileFrame[tile["id"]] = std::make_tuple(frames, speed);
        }

        auto objectGroup = tile["objectgroup"];
        if (!objectGroup.is_null())
        {
            tileset.tileCollider[tile["id"]] = true;
        }

        auto properties = tile["properties"];
        if (!properties.is_null() && properties.is_array())
        {
            for (auto& p: properties)
            {
                auto name = p["name"];
                if (!name.is_null() && name == "zindex")
                {
                    auto zindex = p["value"];
                    LOG(INFO) << "Tile " << tile["id"] << " zindex: " << zindex;
                    tileset.tileZindex[tile["id"]] = zindex;
                }
            }
        }
    }

    return std::make_shared<Tileset>(tileset);
}

AssetLoadResult MapLoader::load(const std::string& assetID, AssetMetadata* metadata) 
{
    LOG(INFO) << "Loading Map: " << assetID;
    std::string path = absl::StrFormat("data/maps/export/%s.json", assetID);
    if (!std::filesystem::exists(path))
    {
        return absl::NotFoundError(absl::StrFormat("Map: %s not found", assetID));
    }

    std::ifstream file(path);
    if (!file.is_open())
    {
        return absl::InternalError("Unable to open the map file");
    }

    nlohmann::json data = nlohmann::json::parse(file);
    Map map;
    map.width = data["width"];
    map.height = data["height"];
    map.tileWidth = data["tilewidth"];
    map.tileHeight = data["tileheight"];
    map.tilesets = std::vector<std::tuple<int, std::string>>();

    for (auto& tileset: data["tilesets"])
    {
        std::string source = tileset["source"];
        size_t lastSlash = source.find_last_of("/\\");
        std::string filename = source.substr(lastSlash + 1);
        size_t lastDot = filename.find_last_of(".");
        std::string tilesetID = filename.substr(0, lastDot);

        map.tilesets.push_back({ tileset["firstgid"], tilesetID });
    }

    // Not checking layer name, type and dimensions.
    // Here we assume every layer has same dimensions and must be
    // drawn at given order.
    for (auto& tiledLayer: data["layers"])
    {
        int width = tiledLayer["width"];
        int height = tiledLayer["height"];
        auto data = tiledLayer["data"];
        std::vector<int> mapLayer;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int i = y * width + x;
                int tileID = data[i];
                mapLayer.push_back(tileID);
            }
        }
        map.layers.emplace_back(mapLayer);
    }

    return std::make_shared<Map>(map);

}
