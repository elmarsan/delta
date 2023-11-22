// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "math/Vec2.h"
#include <vector>

class Animation
{
  public:
    int index;
    int speed;
    std::vector<Point2> frames;

    Animation() = default;
    Animation(int speed, const std::vector<Point2>& frames): index(0), speed(speed), frames(frames) {};
    ~Animation() = default;

    int numFrames() const { return frames.size(); }
    Point2 getFramePoint2(int i) const { return frames[i]; }
    void nextFrame()
    {
        if (index == numFrames() - 1)
            index = 0;
        else
            index++;
    }
};
