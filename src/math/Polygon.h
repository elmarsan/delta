#pragma once

#include "Plane2.h"
#include "Vec2.h"

#include <vector>

using Vertex = std::vector<Vec2>;

struct Polygon: Plane2
{
    Vec2 pos;
    Vertex vertex;

    Polygon(Vec2 pos, Vertex vertex): pos(pos), vertex(std::move(vertex)) {}
    Polygon(): pos(Vec2()), vertex(Vertex {}) {}

    bool contains(const Vec2& p) const override;
    // void draw() const override;
    inline std::size_t size() const { return vertex.size(); }
};
