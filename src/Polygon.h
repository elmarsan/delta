#pragma once

#include "Math.h"
#include "Vec2.h"

#include <vector>

template <typename T>
requires Math::arithmetic<T>
using PolVertex = std::vector<Vec2<T>>;

template <typename T>
requires Math::arithmetic<T>
struct Polygon
{
    PolVertex<T> vertex;

    Polygon(PolVertex<T> vertex): vertex(std::move(vertex)) {}
    Polygon(): vertex(PolVertex<T> {}) {}

    bool isInside(const Point2& point2) const;
    inline std::size_t size() const { return vertex.size(); }
};
