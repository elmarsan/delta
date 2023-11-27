// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Vec2.h"

class Plane2
{
  public:
    virtual bool contains(const Vec2& p) = 0;
    virtual void draw() = 0;
    virtual ~Plane2() = default;
};
