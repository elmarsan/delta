#pragma once

#include <string>
#include <vector>

struct Map
{
    int width, height;
    int tileWidth, tileHeight;
    std::vector<std::vector<int>> layers;
    std::vector<std::tuple<int, std::string>> tilesets;

    int getTileID(int tileGID) const;
};

