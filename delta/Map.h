#pragma once

#include <string>

class Map
{
  public:
    int w;
    int h;
    int tileWidth;
    int tileHeight;

    Map() = default;
    ~Map() = default;

    void load(std::string path);
    void addTile(int x, int y, int tileID, bool hasCollider);
};
