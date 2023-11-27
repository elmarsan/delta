// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Plane2.h"
#include "Vec2.h"

#include <vector>

using Vertex = std::vector<Vec2>;

struct Polygon: Plane2
{
    Vertex vertex;

    Polygon(Vertex vertex): vertex(std::move(vertex)) {}
    Polygon(): vertex(Vertex {}) {}

    bool contains(const Vec2& p) override;
    void draw() override;
    inline std::size_t size() const { return vertex.size(); }
};
