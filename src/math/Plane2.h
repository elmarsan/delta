#pragma once

#include "Vec2.h"

template <typename T>
struct Plane2 
{
    virtual bool contains(const Vec2<T>& p) const = 0; 
    virtual ~Plane2() = default;
};
