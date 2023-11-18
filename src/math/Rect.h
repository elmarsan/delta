#pragma once

#include "math/Plane2.h"

#include <SDL2/SDL_rect.h>

template <typename T>
requires Math::arithmetic<T>
struct Rect: Plane2<T>
{
    T x;
    T y;
    T w;
    T h;

    Rect(T x, T y, T w, T h): x(x), y(y), w(w), h(h) {}

    bool contains(const Vec2<T>& p) const override
    {
        return ( (p.x >= x) && (p.x < (x + w)) &&
                 (p.y >= y) && (p.y < (y + h)) ); 
    }
};


template struct Rect<int>;
