#pragma once

#include "Math.h"
#include "Plane2.h"
#include "Vec2.h"

#include <vector>

template <typename T>
requires Math::arithmetic<T>
using PolVertex = std::vector<Vec2<T>>;

template <typename T>
requires Math::arithmetic<T>
struct Polygon: Plane2<T>
{
    Point2 pos;
    PolVertex<T> vertex;

    Polygon(PolVertex<T> vertex): vertex(std::move(vertex)) {}
    Polygon(): vertex(PolVertex<T> {}) {}

    bool contains(const Vec2<T>& p) const override;
    inline std::size_t size() const { return vertex.size(); }
};
