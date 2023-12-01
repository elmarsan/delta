// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Plane2.h"
#include "Vec2.h"

struct Rect: Plane2
{
    float x;
    float y;
    float w;
    float h;

    Rect(): x(0), y(0), w(0), h(0) {}
    Rect(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {}

    [[nodiscard]] bool contains(const Vec2& p) const override;
    // void draw() override;
};
