#pragma once

#include "Plane2.h"
#include "Vec2.h"

struct Rect: Plane2
{
    float x;
    float y;
    float w;
    float h;

    Rect(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {}

    bool contains(const Vec2& p) const override
    {
        return ((p.x >= x) && (p.x < (x + w)) && (p.y >= y) && (p.y < (y + h)));
    }
};
