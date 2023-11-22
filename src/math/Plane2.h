// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Vec2.h"

struct Plane2 
{
    virtual bool contains(const Vec2& p) const = 0; 
    // virtual void draw() const = 0;
};
