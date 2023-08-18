#pragma once

// struct Animation
// {
//     int index;
//     int frames;
//     int speed;
//     // Animation(int i, int frames, int speed): index(i), frames(frames), speed(speed) {}
// };
//

#include "Vector2D.h"

#include <vector>

class Animation
{
  public:
    int index;
    int speed;
    std::vector<Vector2D> frames;

    Animation() = default;
    Animation(int speed, const std::vector<Vector2D>& frames): index(0), speed(speed), frames(frames) {};
    ~Animation() = default;

    int numFrames() const { return frames.size(); }
    Vector2D getFramePos(int i) const { return frames[i]; }
    void nextFrame()
    {
        if (index == numFrames() - 1)
            index = 0;
        else
            index++;
    }
};
