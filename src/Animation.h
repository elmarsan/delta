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
    Vec2<int> getFramePos(int i) const { return frames[i]; }
    void nextFrame()
    {
        if (index == numFrames() - 1)
            index = 0;
        else
            index++;
    }
};
