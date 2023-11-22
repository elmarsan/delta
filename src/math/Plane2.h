#pragma once

#include "Vec2.h"

struct Plane2 
{
    virtual bool contains(const Vec2& p) const = 0; 
    // virtual void draw() const = 0;
};
